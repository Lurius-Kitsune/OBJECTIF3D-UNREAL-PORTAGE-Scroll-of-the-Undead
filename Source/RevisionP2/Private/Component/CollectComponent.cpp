// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CollectComponent.h"
#include "Entity/PaperPlayer.h"
#include "PaperTileSet.h"
#include "PaperTileMapComponent.h"
#include "Components/AudioComponent.h"
#include "Subsystem/ContextWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperTileLayer.h"

// Sets default values for this component's properties
UCollectComponent::UCollectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCollectComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<APaperPlayer>(GetOwner());
	contextWorldSubsystem = GetWorld()->GetSubsystem<UContextWorldSubsystem>();
}


// Called every frame
void UCollectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCollectComponent::CollectCoins(const int& _value, TObjectPtr<UPaperTileMapComponent> _tileMap, const FVector2D& _playerTilePos)
{
	ClearTile(_tileMap, _playerTilePos);
	contextWorldSubsystem->AddCoins(_value);
	owner->GetAudioComponent()->SetSound(owner->GetPickupSound1());
	owner->GetAudioComponent()->Play();
}

void UCollectComponent::CollectBooks(const int& _value, TObjectPtr<UPaperTileMapComponent> _tileMap, const FVector2D& _playerTilePos)
{
	ClearTile(_tileMap, _playerTilePos);
	contextWorldSubsystem->AddBooks(_value);
	owner->GetAudioComponent()->SetSound(owner->GetPickupSound2());
	owner->GetAudioComponent()->Play();
}

void UCollectComponent::CollectStone(const int& _value, TObjectPtr<UPaperTileMapComponent> _tileMap, const FVector2D& _playerTilePos)
{
	ClearTile(_tileMap, _playerTilePos);
	_tileMap->RebuildCollision();
	contextWorldSubsystem->AddStones(_value);
	owner->GetAudioComponent()->SetSound(owner->GetPickupSound3());
	owner->GetAudioComponent()->Play();
}

void UCollectComponent::ClearTile(TObjectPtr<UPaperTileMapComponent> _tileMap, const FVector2D& _playerTilePos)
{
	_tileMap->SetTile(_playerTilePos.X, _playerTilePos.Y, 0, FPaperTileInfo());
	_tileMap->RebuildCollision();
}

void UCollectComponent::Collect(UPaperTileMapComponent* _tileMap,const FVector2D& _playerTilePos)
{
	for(int _i = 0; _i < 2; _i++)
	{
		FPaperTileInfo _tile = _tileMap->GetTile(_playerTilePos.X, _playerTilePos.Y-_i, 0);

		int _tileIndex = _tile.GetTileIndex();
		if (debugMode)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Tile X: %d Y: %d"), (int32)_playerTilePos.X, (int32)_playerTilePos.Y - _i));
			UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(_tileIndex));
		}

		if (_tileIndex == 1595 || _tileIndex == 1596 || _tileIndex == 1600)
		{
			int _value = _tileIndex == 1595 ? 100 : _tileIndex == 1596 ? 10 : 30;
			CollectCoins(_value, _tileMap, FVector2D(_playerTilePos.X, _playerTilePos.Y - _i));
			break;
		}
		else if (_tileIndex == 1477)
		{
			CollectBooks(1, _tileMap, FVector2D(_playerTilePos.X, _playerTilePos.Y - _i));
		}
		else if (_tileIndex == 1631)
		{
			CollectStone(1, _tileMap, FVector2D(_playerTilePos.X, _playerTilePos.Y - _i));
		}
	}


}