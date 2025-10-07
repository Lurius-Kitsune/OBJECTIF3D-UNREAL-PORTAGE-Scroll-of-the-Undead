// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelMapBuilder.h"
#include "Map/MapDataAssets.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileMapComponent.h"
#include "Components/AudioComponent.h"
#include "PaperTileLayer.h"
#include "Map/LevelMapActor.h"
#include "Subsystem/ContextWorldSubsystem.h"
#include "Components/BillboardComponent.h"

#include "Entity/PaperEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALevelMapBuilder::ALevelMapBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	icon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Icon"));
	icon->SetupAttachment(RootComponent);
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

}

// Called when the game starts or when spawned
void ALevelMapBuilder::BeginPlay()
{
	Super::BeginPlay();
	if (!data) return;
	if (data->music && audioComponent)
	{
		audioComponent->SetSound(data->music);
		audioComponent->Play();
		audioComponent->bIsUISound = true;
		audioComponent->bAllowSpatialization = false;
	}
	playerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	mapActor = GetWorld()->SpawnActor<ALevelMapActor>(actorBlueprint, FVector::ZeroVector, FRotator::ZeroRotator);
	GetWorld()->GetSubsystem<UContextWorldSubsystem>()->SetMapActor(this);
	TArray<FString> _out = ParseStringFromData(data->mapDataString);
	int _lineCount = _out.Num();
	for(int _i = 0; _i < _lineCount; _i++)
	{
		if (_out[_i].StartsWith("TILE"))
		{
			TArray<FString> _temp;
			_out[_i].ParseIntoArray(_temp, TEXT(" "), true);
			if (collectibleList.Contains(_temp[1]))
			{
				
				collectible.Add(_out[_i]);
				continue;
			}
			tile.Add(_out[_i]);
		}
		else if (_out[_i].StartsWith("PLAYER"))
		{
			player.Add(_out[_i]);
		}
		else if (_out[_i].StartsWith("ENEMY"))
		{
			enemy.Add(_out[_i]);
		}
	}

	if (!mapActor) return;
	TObjectPtr<UPaperTileMap> _map = CreateInstanceTileMap(mapActor->GetRenderComponent());
	TObjectPtr<UPaperTileMap> _collectMap = CreateInstanceTileMap(mapActor->GetCollectMapComponent());
	CreateTileMapFromData(_map);
	GenerateBackgroundLayer(_map);
	CreateCollectibleMap(_collectMap);
	mapActor->GetRenderComponent()->RebuildCollision();
	mapActor->GetCollectMapComponent()->RebuildCollision();
	PlaceMap(_map);
	PlacePlayer(_map);

	for (int _i = 0; _i < enemy.Num(); _i++)
	{
		TArray<FString> _temp;
		enemy[_i].ParseIntoArray(_temp, TEXT(" "), true);
		if (_temp.Num() < 4) continue;
		EEnemyType _type = EEnemyType::None;
		if (_temp[1] == "Skeleton") _type = EEnemyType::Skeleton;
		else if (_temp[1] == "Goblin") _type = EEnemyType::Goblin;
		if (_type == EEnemyType::None) continue;
		TSubclassOf<APaperEnemy> _blueprint = *enemyBlueprints.Find(_type);
		if (!_blueprint) continue;
		TObjectPtr<APaperEnemy> _enemyActor = GetWorld()->SpawnActor<APaperEnemy>(_blueprint, FVector::ZeroVector, FRotator::ZeroRotator);
		if (!_enemyActor) continue;
		_enemyActor->SetActorRotation(FRotator(0, 0, 90));
		FVector2D _spawnPos = FVector2D(FCString::Atoi(*_temp[2]), FCString::Atoi(*_temp[3]));
		PlaceEntity(_enemyActor, _map, _spawnPos);
	}
}

TArray<FString> ALevelMapBuilder::ParseStringFromData(const FString& _data)
{
	if (_data.IsEmpty()) return TArray<FString>();
	TArray<FString> _out;
	_data.ParseIntoArrayLines(_out);
	return _out;
}

TObjectPtr<UPaperTileMap> ALevelMapBuilder::CreateInstanceTileMap(TObjectPtr<UPaperTileMapComponent> _component)
{
	TObjectPtr<UPaperTileMap> _map = NewObject<UPaperTileMap>(_component);
	_map->SetFlags(RF_Transactional);
	_map->InitializeNewEmptyTileMap();
	_map->ResizeMap(data->mapSize.X, data->mapSize.Y);
	_component->SetTileMap(_map);
	return _map;
}

TObjectPtr<UPaperTileMap> ALevelMapBuilder::CreateTileMapFromData(TObjectPtr<UPaperTileMap> _map)
{
	if (!data) return nullptr;
	TObjectPtr<UPaperTileSet> _tileSet = data->tileSet;
	if (!_tileSet) return nullptr;
	int _numberTile = tile.Num();
	for(int _i = 0; _i < _numberTile; _i++)
	{
		GenerateCell(tile[_i], 0, _map);
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Map Created: %d x %d"), (int32)data->mapSize.X, (int32)data->mapSize.Y));
	return _map;
}

TObjectPtr<UPaperTileMap> ALevelMapBuilder::CreateCollectibleMap(TObjectPtr<UPaperTileMap> _map)
{
	if (!data) return nullptr;
	TObjectPtr<UPaperTileSet> _tileSet = data->tileSet;
	if (!_tileSet) return nullptr;
	int _numberTile = collectible.Num();
	for(int _i = 0; _i < _numberTile; _i++)
	{
		GenerateCell(collectible[_i], 0, _map);
	}
	return _map;
}

void ALevelMapBuilder::GenerateBackgroundLayer(TObjectPtr<UPaperTileMap> _map)
{
	TArray<FString> _out = ParseStringFromData(data->mapBackgroundDataString);
	if (_out.IsEmpty()) return;
	int _lineCount = _out.Num();
	_map->AddNewLayer(1);
	_map->TileLayers[1]->SetLayerCollides(false);
	TObjectPtr<UPaperTileSet> _tileSet = data->tileSet;
	for(int _i = 0; _i < _lineCount; _i++)
	{
		GenerateCell(_out[_i], 1, _map);
	}
}

void ALevelMapBuilder::GenerateCell(const FString& _data, const int& _layer, TObjectPtr<UPaperTileMap> _map)
{
	TObjectPtr<UPaperTileSet> _tileSet = data->tileSet;
	if (!_tileSet) return;
	TArray<FString> _out;
	_data.ParseIntoArray(_out, TEXT(" "), true);
	if (_out.Num() < 4) return;
	int _index = FCString::Atoi(*_out[1]);
	int _x = FCString::Atoi(*_out[2]);
	int _y = FCString::Atoi(*_out[3]);
	_map->TileLayers[_layer]->SetCell(_x, _y, GenerateTileInfo(_index, _tileSet));
}

FPaperTileInfo ALevelMapBuilder::GenerateTileInfo(const int& _index, const TObjectPtr<UPaperTileSet>& _tileSet)
{
	if (!_tileSet) return FPaperTileInfo();
	FPaperTileInfo _tileInfo;
	_tileInfo.TileSet = _tileSet;
	_tileInfo.PackedTileIndex = _index;
	return _tileInfo;
}

void ALevelMapBuilder::PlaceMap(const TObjectPtr<UPaperTileMap>& _map)
{
	if (!mapActor) return;
	mapActor->SetActorRotation(FRotator(0, 0, 90));
	mapActor->SetActorLocation(FVector(0,_map->MapHeight * _map->TileHeight + _map->TileHeight/2,0));
}

void ALevelMapBuilder::PlacePlayer(const TObjectPtr<UPaperTileMap>& _map)
{
	if (!mapActor || !playerActor) return;
	if (player.IsEmpty()) return;
	TArray<FString> _out;
	player[0].ParseIntoArray(_out, TEXT(" "), true);
	FVector2D _spawnPos = FVector2D(FCString::Atoi(*_out[1]), FCString::Atoi(*_out[2]));
	PlaceEntity(Cast<APaperCharacterActor>(playerActor), _map, _spawnPos);
	/*FVector _localSpawn = _map->GetTileCenterInLocalSpace(FCString::Atoi(*_out[1]), FCString::Atoi(*_out[2]));
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Spawn Pos X: %f Y: %f"), _localSpawn.X, _localSpawn.Y));
	playerActor->SetActorLocation(FVector(_localSpawn.X, _map->MapHeight * _map->TileHeight - _localSpawn.Y, 0));
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Actor Pos X: %f Y: %f"), playerActor->GetActorLocation().X, playerActor->GetActorLocation().Y));*/
}

void ALevelMapBuilder::PlaceEntity(TObjectPtr<APaperCharacterActor> _actor, const TObjectPtr<UPaperTileMap>& _map, const FVector2D& _cords)
{
	if (!mapActor || !_actor ) return;
	FVector2D _spawnCord = FVector2D(_cords.X, _map->MapHeight * _map->TileHeight - _cords.Y);
	_actor->SetActorLocation(FVector(_spawnCord, 0));
	_actor->SetPosition(_spawnCord.X, _spawnCord.Y);
}

// Called every frame
void ALevelMapBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

