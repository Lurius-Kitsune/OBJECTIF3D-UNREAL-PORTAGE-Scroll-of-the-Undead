// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/BaseEntity.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	position = FVector2D(GetActorLocation());
}

// Called every frame
void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float _dt = DeltaTime;
	float _gravity = 512.0f;
	Accelerate(0, -_gravity);
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Aceleration : x: ") + FString::SanitizeFloat(acceleration.X) + TEXT(" y: ") + FString::SanitizeFloat(acceleration.Y));
	AddVelocity(acceleration.X * _dt, acceleration.Y * _dt);
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Velocity : x: ") + FString::SanitizeFloat(velocity.X) + TEXT(" y: ") + FString::SanitizeFloat(velocity.Y));
	SetAcceleration(0.0f, 0.0f);
	FVector2D _frictionValue = FVector2D::ZeroVector;
	_frictionValue = friction;

	const float& _frictionX = (_frictionValue.X * speed.X) * _dt;
	const float& _frictionY = (_frictionValue.Y * speed.Y) * _dt;
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Friction : x: ") + FString::SanitizeFloat(_frictionX) + TEXT(" y: ") + FString::SanitizeFloat(_frictionY));
	ApplyFriction(_frictionX, _frictionY);
	FVector2D _deltaPos = velocity * _dt;
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("DeltaPos : x: ") + FString::SanitizeFloat(_deltaPos.X) + TEXT(" y: ") + FString::SanitizeFloat(_deltaPos.Y));
	Move(_deltaPos);
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
	if (_state != state) {
		onStateChanged.Broadcast(_state);
	}
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

void ABaseEntity::Move(const FVector2D& _movement)
{
	positionOld = position;
	position += _movement;
	FHitResult _result = FHitResult();
	SetActorLocation(FVector(position.X, positionOld.Y, -1.0f), true, &_result);
	if (_result.bBlockingHit) {
		velocity.X = 0.0f;
		position.X = positionOld.X;
	}
	SetActorLocation(FVector(position.X, position.Y, -1.0f), true, &_result);
	if (_result.bBlockingHit) {
		velocity.Y = 0.0f;
		position.Y = positionOld.Y;
	}
	if (position.X < 0) {
		position.X = 0;
	}
	if (position.Y < 0) {
		position.Y = 0;
	}
	/*sf::Vector2u mapSize = m_entityManager->GetContext()->m_gameMap->GetMapSize();
	
	else if (m_position.x > (mapSize.x) * Sheet::Tile_Size) {
		m_position.x = (mapSize.x) * Sheet::Tile_Size;
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
