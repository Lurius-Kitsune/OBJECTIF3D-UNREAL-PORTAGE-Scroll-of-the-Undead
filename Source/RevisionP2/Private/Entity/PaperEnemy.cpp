
#include "Entity/PaperEnemy.h"
#include "Entity/PaperPlayer.h"
#include "Components/BoxComponent.h"
#include "Subsystem/EntityManager.h"

APaperEnemy::APaperEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APaperEnemy::BeginPlay()
{
	Super::BeginPlay();
	hitbox->OnComponentBeginOverlap.AddDynamic(this, &APaperEnemy::OnEntityCollision);
	onDeath.AddDynamic(this, &APaperEnemy::OnDeath);
}

void APaperEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hasDestination) {
		if (abs(destination.X - position.X) < 16) { hasDestination = false; return; }
		if (destination.X - position.X > 0) { Move(EEntityDirection::Right); }
		else { Move(EEntityDirection::Left); }
		//Check collision with block / hitbox
		if (collidingOnX) { hasDestination = false; }
		return;
	}
	int _random = rand() % 100 + 1;
	if (_random != 100) { return; }
	int _newX = rand() % 65 + 0;
	if (rand() % 2) { _newX = -_newX; }
	destination.X = position.X + _newX * 32;
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
	// check if hitbox is not the attackhitbox
	if (_otherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1) return;
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

void APaperEnemy::OnDeath()
{
	entityManager->RemoveAndDestroy(this);
}

