// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperCharacterActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APaperCharacterActor::APaperCharacterActor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	attackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitbox"));
	animation = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("Animation"));
	animation->InitRenderComponent(sprite);
	attackHitbox->SetupAttachment(hitbox);
	sprite->SetupAttachment(hitbox);
}

// Called when the game starts or when spawned
void APaperCharacterActor::BeginPlay()
{
	Super::BeginPlay();
	attackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bool _isHiddennGame = !debugMode;
	attackHitbox->SetHiddenInGame(_isHiddennGame);
	hitbox->SetHiddenInGame(_isHiddennGame);
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
	onMovement.Broadcast(_movement);
	if (debugMode) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Move : ") + FString::SanitizeFloat(_movement.X));
	if (_movement.X < 0) 
	{ 
		Move(EEntityDirection::Left);
	}
	else if (_movement.X > 0)
	{
		Move(EEntityDirection::Right);
	}
}

void APaperCharacterActor::Move(const EEntityDirection& _value)
{
	if (GetState() == EEntityState::Dying) { return; }
	EEntityDirection _dirOld = direction;
	Accelerate(_value == EEntityDirection::Left  ? -speed.X : speed.X, 0);
	SetDirection(_value);
	if (GetState() == EEntityState::Idle) { SetState(EEntityState::Walking); }
}

void APaperCharacterActor::Jump()
{
	if (debugMode) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Jump"));
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
	if (debugMode) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GetHurt : ") + FString::FromInt(_damage));
	if (GetState() == EEntityState::Dying || GetState() == EEntityState::Hurt) { return; }
	currentHitPoints = (currentHitPoints - _damage > 0 ? currentHitPoints - _damage : 0);
	//if (type == EEntityType::Player)
		//entityManager->GetContext()->m_characterCurrentHealth = currentHitPoints;
	if (currentHitPoints > 0) 
	{ 
		SetState(EEntityState::Hurt); 
		if (debugMode) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("HURT entity :") + name + TEXT(" HP : ") + FString::FromInt(currentHitPoints) + TEXT("/") + FString::FromInt(hitPoints), true, true, FLinearColor::Red);
	}
	else {
		SetState(EEntityState::Dying);
	}
}

void APaperCharacterActor::SetDirection(const EEntityDirection& _dir)
{
	if (_dir != direction)
	{
		if (debugMode) UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Direction changed"));
		attackHitbox->SetRelativeLocation(attackHitbox->GetRelativeLocation() * FVector(-1, 1, 1));
		onDirectionChanged.Broadcast(_dir);
		direction = _dir;
	}
}

void APaperCharacterActor::ToogleAttackHitbox(const bool _enable)
{
	attackHitbox->SetCollisionEnabled(_enable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	if (debugMode && _enable) UKismetSystemLibrary::DrawDebugBox(GetWorld(), attackHitbox->GetComponentLocation(), attackHitbox->GetScaledBoxExtent(), FColor::Red, FRotator::ZeroRotator, 1.0f, 2.0f);
}


