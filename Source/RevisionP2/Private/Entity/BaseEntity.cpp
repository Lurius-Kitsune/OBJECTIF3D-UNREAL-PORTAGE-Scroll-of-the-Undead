// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/BaseEntity.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystem/EntityManager.h"
#include "Map/LevelMapBuilder.h"
#include "Map/LevelMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "Subsystem/ContextWorldSubsystem.h"

// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	hitbox->SetupAttachment(RootComponent);
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));


}

// Called when the game starts or when spawned
void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	position = FVector2D(GetActorLocation());
	entityManager = GetWorld()->GetSubsystem<UEntityManager>();
	if (entityManager) entityManager->Add(this);
	contextManager = GetWorld()->GetSubsystem<UContextWorldSubsystem>();
}

// Called every frame
void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!contextManager) return;
	float _gravity = contextManager->GetMapActor()->GetGravity();
	Accelerate(0, -_gravity);
	AddVelocity(acceleration.X * DeltaTime, acceleration.Y * DeltaTime);
	SetAcceleration(0.0f, 0.0f);
	FVector2D _frictionValue = FVector2D::ZeroVector;
	_frictionValue = contextManager->GetMapActor()->GetFriction();

	const float& _frictionX = (_frictionValue.X * speed.X) * DeltaTime;
	const float& _frictionY = (_frictionValue.Y * speed.Y) * DeltaTime;
	ApplyFriction(_frictionX, _frictionY);
	FVector2D _deltaPos = velocity * DeltaTime;
	Move(_deltaPos);
}

// Called to bind functionality to input
void ABaseEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEntity::SetPosition(float _x, float _y)
{
	position = FVector2D(_x, _y);
	SetPosition(position);
}

void ABaseEntity::SetPosition(const FVector2D& _pos)
{
	position = _pos;
	positionOld = position;
	SetActorLocation(FVector(position, 0.0f));
}

void ABaseEntity::SetSize(float _x, float _y)
{
	hitbox->SetBoxExtent(FVector(_x * 0.5f, _y * 0.5f, 50.0f));
}

void ABaseEntity::SetState(const EEntityState _state)
{

	if (state == EEntityState::Dying) { return; }
	if (_state != state) {
		onStateChanged.Broadcast(_state);
	}
	state = _state;

	if (_state == EEntityState::Attacking) {
		if (audioComponent->Sound == attackSound || !audioComponent->IsPlaying())
		{
			audioComponent->SetSound(attackSound);
			audioComponent->Play();
		}
	}
	else if (_state == EEntityState::Dying) {
		audioComponent->SetSound(dieSound);
		audioComponent->Play();
	}
	else if (_state == EEntityState::Hurt) {
		audioComponent->SetSound(hurtSound);
		audioComponent->Play();
	}
}

void ABaseEntity::Move(const FVector2D& _movement)
{
	positionOld = position;
	position += _movement;
	FHitResult _result = FHitResult();
	SetActorLocation(FVector(position.X, positionOld.Y, zOffset), true, &_result);
	collidingOnX = _result.bBlockingHit;
	if (_result.bBlockingHit) {
		velocity.X = 0.0f;
		position.X = positionOld.X;
	}
	SetActorLocation(FVector(position.X, position.Y, zOffset), true, &_result);
	if (_result.bBlockingHit) {
		velocity.Y = 0.0f;
		position.Y = positionOld.Y;
	}
	if (position.X < 0) {
		velocity.X = 0.0f;
		position.X = 0;
	}
	if (position.Y < 0) {
		velocity.Y = 0.0f;
		position.Y = 0;
	}
	
	TObjectPtr<ALevelMapBuilder> _mapActor = GetWorld()->GetSubsystem<UContextWorldSubsystem>()->GetMapActor();

	FVector2D _mapSize = _mapActor->GetMapActor()->GetRealMapSize();
	if (position.X > _mapSize.X ) {
		position.X = _mapSize.X;
		velocity.X = 0.0f;
	}

	
	/*else if (m_position.y > (mapSize.y + 1) * Sheet::Tile_Size) {
		m_position.y = (mapSize.y + 1) * Sheet::Tile_Size;
		SetState(EntityState::Dying);
	}*/

}

void ABaseEntity::AddVelocity(float _x, float _y)
{
	velocity += FVector2D(_x, _y);
	if (abs(velocity.X) > maxVelocity.X) {
		if (velocity.X < 0) { velocity.X = -maxVelocity.X; }
		else { velocity.X = maxVelocity.X; }
	}

	if (abs(velocity.Y) > maxVelocity.Y) {
		if (velocity.Y < 0) { velocity.Y = -maxVelocity.Y; }
		else { velocity.Y = maxVelocity.Y; }
	}
}

void ABaseEntity::Accelerate(float _x, float _y)
{
	acceleration += FVector2D(_x, _y);
}

void ABaseEntity::SetAcceleration(float _x, float _y)
{
	acceleration = FVector2D(_x, _y);
}

void ABaseEntity::ApplyFriction(float _x, float _y)
{
	if (velocity.X != 0.0f) {
		if (abs(velocity.X) - _x < 0.0f) { velocity.X = 0.0f; }
		else if (velocity.X < 0.0f) { velocity.X += _x; }
		else { velocity.X -= _x; }
	}
	if (velocity.Y != 0.0f) {
		if (abs(velocity.Y) - _y < 0.0f) { velocity.Y = 0.0f; }
		else if (velocity.Y < 0.0f) { velocity.Y += _y; }
		else { velocity.Y -= _y; }
	}
}

void ABaseEntity::OnEntityCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
}
