#include "Entity/PaperPlayer.h"
#include "Map/LevelMapActor.h"
#include "Map/LevelMapBuilder.h"
#include "Component/CollectComponent.h"
#include <EnhancedInputComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PaperTileMapComponent.h"
#include "Components/BoxComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "Subsystem/ContextWorldSubsystem.h"
#include <EnhancedInputSubsystems.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
APaperPlayer::APaperPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springarm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	backgroundImageComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundImage"));
	springarm->SetupAttachment(hitbox);
	camera->SetupAttachment(springarm);
	backgroundImageComponent->SetupAttachment(camera);
	collectComponent = CreateDefaultSubobject<UCollectComponent>(TEXT("CollectComponent"));


	hitPoints = 5;
	currentHitPoints = hitPoints;
	name = "Player";
	type = EEntityType::Player;
	speed = FVector2D(1024.0f, 128.0f);
	jumpVelocity = 300.0f;
	maxVelocity = FVector2D(200.0f, 1024.0f);
}

// Called when the game starts or when spawned
void APaperPlayer::BeginPlay()
{
	Super::BeginPlay();
	Init();
	SetupBackgroundImage();
	SetupCamera();
	if(attackHitbox) attackHitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnEntityCollision);
	if (hitbox) hitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnHitboxCollision);
}

void APaperPlayer::Init()
{
	// On récupère le manager du premier joeur LOCAL
	ULocalPlayer* _localPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* _sys = _localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_sys)
	{
		UE_LOG(LogTemp, Error, TEXT("No EnhancedInputLocalPlayerSubsystem found"));
		return;
	}
	// On donne le contexte de mapping
	_sys->AddMappingContext(inputData.inputMapping, 0);
}

void APaperPlayer::SetupBackgroundImage()
{
	TObjectPtr<UTexture2D> _backgroundImage = contextManager->GetMapActor()->GetMapData()->backgroundImage;
	if (!_backgroundImage) return;
	UMaterialInstanceDynamic* _mat = backgroundImageComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (!_mat) return;
	_mat->SetTextureParameterValue(FName("Texture"), _backgroundImage);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Background Image Set")), true, true, FLinearColor::Green);
}

void APaperPlayer::SetupCamera()
{
	TObjectPtr<ALevelMapActor> _map = contextManager->GetMapActor()->GetMapActor();
	if (!_map) return;
	TObjectPtr<UPaperTileMapComponent> _mapComp = _map->GetRenderComponent();
	const float _halfWidth = camera->OrthoWidth;              // world units
	const float _halfHeight = _halfWidth / camera->AspectRatio;
	FVector2D _cameraView = FVector2D(_halfWidth, _halfHeight);
	FVector _mapPos = _map->GetActorLocation();
	TObjectPtr<UPaperTileMap> _tileMap = _mapComp->TileMap;
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Map Size X: %f Y: %f"), _map->GetRealMapSize().X, _map->GetRealMapSize().Y), true, true, FLinearColor::Green);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Camera View X: %f Y: %f"), _cameraView.X, _cameraView.Y), true, true, FLinearColor::Green);
	float _minX = _mapPos.X + _cameraView.X + _mapComp->TileMap->TileWidth * 2;
	float _maxX = _mapPos.X + _map->GetRealMapSize().X - _cameraView.X - _mapComp->TileMap->TileWidth * 2;
	float _minY = _mapPos.Y - _map->GetRealMapSize().Y + _cameraView.Y - _mapComp->TileMap->TileWidth;
	float _maxY = _mapPos.Y - _cameraView.Y + _mapComp->TileMap->TileHeight;

	cameraDetails.minCameraOffset = FVector2D(_minX, _minY);
	cameraDetails.maxCameraOffset = FVector2D(_maxX, _maxY);
}

void APaperPlayer::UpdateCameraPosition()
{
	if (!camera) return;
	FVector _newCameraPos = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	_newCameraPos.X = FMath::Clamp(_newCameraPos.X, cameraDetails.minCameraOffset.X, cameraDetails.maxCameraOffset.X);
	_newCameraPos.Y = FMath::Clamp(_newCameraPos.Y, cameraDetails.minCameraOffset.Y, cameraDetails.maxCameraOffset.Y);
	camera->SetWorldLocation(_newCameraPos);
}


// Called every frame
void APaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCameraPosition();
}

// Called to bind functionality to input
void APaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _inputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!_inputCompo) return;
	_inputCompo->BindAction(inputData.moveAction, ETriggerEvent::Triggered, this, &APaperPlayer::Move);
	_inputCompo->BindAction(inputData.moveAction, ETriggerEvent::Completed, this, &APaperPlayer::Move);
	_inputCompo->BindAction(inputData.jumpAction, ETriggerEvent::Started, this, &APaperCharacterActor::Jump);
	_inputCompo->BindAction(inputData.attackAction, ETriggerEvent::Triggered, this, &APaperCharacterActor::Attack);
}

void APaperPlayer::OnEntityCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
	if (state == EEntityState::Dying) { return; }
	//if (l_attack) {
		if (state != EEntityState::Attacking) { return; }
		TObjectPtr<APaperCharacterActor> _char = Cast<APaperCharacterActor>(_other);
		if (!_char) { return; };
		if (_char->GetType() != EEntityType::Enemy &&
			_char->GetType() != EEntityType::Player)
		{
			return;
		}
		_char->GetHurt(1);
		if (_char->GetState() == EEntityState::Dying)
		{
			_char->SetDirection(direction == EEntityDirection::Left ? EEntityDirection::Right : EEntityDirection::Left);
		}
		if (position.X> _char->GetPosition().X) {
			_char->AddVelocity(-32, 0);
		}
		else {
			_char->AddVelocity(32, 0);
		}
}

void APaperPlayer::OnHitboxCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
	TObjectPtr<UPaperTileMapComponent> _timeMapComponent = Cast<UPaperTileMapComponent>(_otherComp);
	if (!_timeMapComponent) return;
	TObjectPtr<UPaperTileMap> _tileMap = _timeMapComponent->TileMap;
	int _x = 0;
	int _y = 0;
	FVector _relativePlayerTransform = _timeMapComponent->GetComponentTransform().InverseTransformPosition(_me->GetComponentLocation());
	_tileMap->GetTileCoordinatesFromLocalSpacePosition(_relativePlayerTransform, _x, _y);
	if(debugMode) UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Pos in Tile X: %i Y: %i"), _x, _y));
	collectComponent->Collect(_timeMapComponent, FVector2D(_x, _y));
}

