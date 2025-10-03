// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/PaperEnemy.h"
#include "Components/BoxComponent.h"
#include "Entity/PaperPlayer.h"

APaperEnemy::APaperEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APaperEnemy::BeginPlay()
{
	Super::BeginPlay();
	attackHitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperEnemy::OnEntityCollision);
}

void APaperEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hasDestination) {
		if (abs(destination.X - position.X) < 16) { hasDestination = false; return; }
		if (destination.X - position.X > 0) { Move(EEntityDirection::Right); }
		else { Move(EEntityDirection::Left); }
		//Check collision with block / hitbox
		
		bool _collidingOnX = (position.X == positionOld.X);
		if (_collidingOnX) { hasDestination = false; }
		return;
	}
	int random = rand() % 100 + 1;
	if (random != 100) { return; }
	int newX = rand() % 65 + 0;
	if (rand() % 2) { newX = -newX; }
	destination.X = position.X + newX;
	if (destination.X < 0) { destination.X = 0; }
	hasDestination = true;
}

void APaperEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APaperEnemy::OnEntityCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
	if (state == EEntityState::Dying) { return; }
	//if (attack) { return; }
	TObjectPtr<APaperPlayer> _player = Cast<APaperPlayer>(_other);
	if (!_player) { return; }
	SetState(EEntityState::Attacking);
	_player->GetHurt(1);
	if (position.X > _player->GetPosition().X) {
		_player->AddVelocity(-speed.X, 0);
		SetDirection(EEntityDirection::Left);
	}
	else {
		_player->AddVelocity(speed.Y, 0);
		SetDirection(EEntityDirection::Right);
	}
}

