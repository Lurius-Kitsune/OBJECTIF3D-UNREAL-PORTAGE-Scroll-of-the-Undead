// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperPlayer.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Components/BoxComponent.h"

// Sets default values
APaperPlayer::APaperPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springarm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	springarm->SetupAttachment(hitbox);
	camera->SetupAttachment(springarm);


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

	hitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperPlayer::OnEntityCollision);
}

// Called every frame
void APaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

