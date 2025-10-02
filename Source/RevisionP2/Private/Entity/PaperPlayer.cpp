// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperPlayer.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

// Sets default values
APaperPlayer::APaperPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	_inputCompo->BindAction(inputData.jumpAction, ETriggerEvent::Started, this, &APaperCharacterActor::Jump);
	_inputCompo->BindAction(inputData.attackAction, ETriggerEvent::Triggered, this, &APaperCharacterActor::Attack);
}

