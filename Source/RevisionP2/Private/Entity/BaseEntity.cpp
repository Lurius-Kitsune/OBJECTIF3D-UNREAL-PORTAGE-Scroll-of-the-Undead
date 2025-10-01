// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/BaseEntity.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	hitbox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	SetPosition
}

// Called every frame
void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//[U]
//inline FVector2D ABaseEntity::GetSize() const { return hitbox->GetScaledBoxExtent() * 2.0f; }

void ABaseEntity::SetPosition(float _x, float _y)
{
	position = FVector2D(_x, _y);
	SetActorLocation(FVector(position, 0.0f));
}

void ABaseEntity::SetPosition(const FVector2D& _pos)
{
	position = _pos;
	SetActorLocation(FVector(position, 0.0f));
}

void ABaseEntity::SetSize(float _x, float _y)
{
	hitbox->SetBoxExtent(FVector(_x * 0.5f, _y * 0.5f, 50.0f));
}

void ABaseEntity::SetState(const EEntityState& _state)
{
	if (state == EEntityState::Dying) { return; }
	state = _state;

	if (_state == EEntityState::Attacking) {
		//if (soundAttack.getStatus() != sf::SoundSource::Status::Playing)
		//	m_soundAttack.play();
	}
	else if (_state == EEntityState::Dying) {
		//m_soundDie.play();
	}
	else if (_state == EEntityState::Hurt) {
		//m_soundHurt.play();
	}
}

void ABaseEntity::Move(float _x, float _y)
{
	positionOld = position;
	position += FVector2D(_x, _y);
	SetActorLocation(FVector(position, 0.0f));
	/*sf::Vector2u mapSize = m_entityManager->GetContext()->m_gameMap->GetMapSize();
	if (m_position.x < 0) {
		m_position.x = 0;
	}
	else if (m_position.x > (mapSize.x) * Sheet::Tile_Size) {
		m_position.x = (mapSize.x) * Sheet::Tile_Size;
	}

	if (m_position.y < 0) {
		m_position.y = 0;
	}
	else if (m_position.y > (mapSize.y + 1) * Sheet::Tile_Size) {
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
