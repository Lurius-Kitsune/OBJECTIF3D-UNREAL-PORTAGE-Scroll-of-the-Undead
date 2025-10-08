// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperPlayer.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include "PaperTileMapActor.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Component/CollectComponent.h"
#include "PaperTileMapComponent.h"
#include "Subsystem/ContextWorldSubsystem.h"
#include "Map/LevelMapBuilder.h"
#include "Map/LevelMapActor.h"
#include "Components/BoxComponent.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
APaperPlayer::APaperPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springarm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	springarm->SetupAttachment(hitbox);
	camera->SetupAttachment(springarm);

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

	attackHitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnEntityCollision);
	hitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnHitboxCollision);
}

// Called every frame
void APaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//get size of the view for test
	UContextWorldSubsystem* contextManager = GetWorld()->GetSubsystem<UContextWorldSubsystem>();
	if (!contextManager) return;
	FMinimalViewInfo _cameraView;
	camera->GetCameraView(DeltaTime, _cameraView);
	float _viewWidth = _cameraView.FOV;
	float _viewHeight = camera->AspectRatio;
	
	//FVector _cameraView = camera->GetComponentLocation();
	/*FMinimalViewInfo _cameraView;
	camera->GetCameraView(DeltaTime, _cameraView);
	FVector _cameraLocation = _cameraView.Location;
	FVector _mapPos = contextManager->GetMapActor()->GetMapActor()->GetActorLocation();
	FVector2D _realMapSize = contextManager->GetMapActor()->GetMapActor()->GetRealMapSize();

	float _minX = _mapPos.X + (_cameraLocation.X / 2);
	float _maxX = _mapPos.X + _realMapSize.X - (_cameraLocation.X / 2);
	float _maxY = _mapPos.Y - (_cameraLocation.Y / 2);
	float _minY = _mapPos.Y - _realMapSize.Y - (_cameraLocation.Y / 2);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Cam MinX: %f MaxX: %f MinY: %f MaxY: %f"), _minX, _maxX, _minY, _maxY), true, true, FLinearColor::Green);

	FVector _camLocation = camera->GetComponentLocation();
	_camLocation.X = FMath::Clamp(_camLocation.X, _minX, _maxX);
	_camLocation.Y = FMath::Clamp(_camLocation.Y, _minY, _maxY);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Cam Pos X: %f Y: %f"), _camLocation.X, _camLocation.Y), true, true, FLinearColor::Yellow);

	camera->SetWorldLocation(_camLocation);*/
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
	//}
	//else {
	//	// Other behavior.
	//}
}

void APaperPlayer::OnHitboxCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
	TObjectPtr<UPaperTileMapComponent> _timeMapComponent = Cast<UPaperTileMapComponent>(_otherComp);
	if (!_timeMapComponent) return;
	TObjectPtr<UPaperTileMap> _tileMap = _timeMapComponent->TileMap;
	int _x, _y = 0;
	FVector _relativePlayerTransform = _timeMapComponent->GetComponentTransform().InverseTransformPosition(_me->GetComponentLocation());
	_tileMap->GetTileCoordinatesFromLocalSpacePosition(_relativePlayerTransform, _x, _y);
	if(debugMode) UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Pos in Tile X: %i Y: %i"), _x, _y));
	collectComponent->Collect(_timeMapComponent, FVector2D(_x, _y));
}

