// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperCharacterActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
APaperCharacterActor::APaperCharacterActor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	attackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitbox"));
	attackHitbox->SetupAttachment(hitbox);
	sprite->SetupAttachment(hitbox);
}

// Called when the game starts or when spawned
void APaperCharacterActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APaperCharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetState() != EEntityState::Dying && GetState() != EEntityState::Attacking && GetState() != EEntityState::Hurt) {
		if (abs(velocity.Y) >= 0.001f) {
			SetState(EEntityState::Jumping);
		}
		else if (abs(velocity.X) >= 0.1f) {
			SetState(EEntityState::Walking);
		}
		else {
			SetState(EEntityState::Idle);
		}
	}
	else if (GetState() == EEntityState::Dying) {
		/*if (!m_spriteSheet.GetCurrentAnim()->IsPlaying()) {
			m_entityManager->Remove(m_id);
		}*/
	}
}

// Called to bind functionality to input
void APaperCharacterActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APaperCharacterActor::Move(const FInputActionValue& _value)
{
	if (GetState() == EEntityState::Dying) { return; }
	const FVector2D& _movement = _value.Get<FVector2D>();
	// Tourner le sprite selon la direction -> event? 
	onMovement.Broadcast(_movement);
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Move : ") + FString::SanitizeFloat(_movement.X));
	if (_movement.X < 0) { Accelerate(-speed.X, 0); }
	else { Accelerate(speed.X, 0); }
	if (GetState() == EEntityState::Idle) { SetState(EEntityState::Walking); }
}

void APaperCharacterActor::Jump()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Jump"));
	if (GetState() == EEntityState::Dying || GetState() == EEntityState::Jumping || GetState() == EEntityState::Hurt) { return; }
	SetState(EEntityState::Jumping);
	AddVelocity(0, jumpVelocity);
}

void APaperCharacterActor::Attack()
{
	if (GetState() == EEntityState::Dying || GetState() == EEntityState::Jumping ||
		GetState() == EEntityState::Hurt || GetState() == EEntityState::Attacking)
	{
		return;
	}
	SetState(EEntityState::Attacking);
}

void APaperCharacterActor::GetHurt(const int& _damage)
{
	if (GetState() == EEntityState::Dying || GetState() == EEntityState::Hurt) { return; }
	currentHitPoints = (currentHitPoints - _damage > 0 ? currentHitPoints - _damage : 0);
	if (type == EEntityType::Player)
		//entityManager->GetContext()->m_characterCurrentHealth = currentHitPoints;
	if (currentHitPoints) { SetState(EEntityState::Hurt); }
	else {
		SetState(EEntityState::Dying);
	}
}


