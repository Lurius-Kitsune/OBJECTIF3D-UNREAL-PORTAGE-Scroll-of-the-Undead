// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelMapBuilder.h"
#include "Map/MapDataAssets.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "Map/LevelMapActor.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALevelMapBuilder::ALevelMapBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	icon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Icon"));
	icon->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALevelMapBuilder::BeginPlay()
{
	Super::BeginPlay();
	mapActor = GetWorld()->SpawnActor<ALevelMapActor>(actorBlueprint.Get(), FVector::ZeroVector, FRotator::ZeroRotator);
	TArray<FString> _out = ParseStringFromData(data->mapDataString);
	int _lineCount = _out.Num();
	for(int _i = 0; _i < _lineCount; _i++)
	{
		if (_out[_i].StartsWith("TILE"))
		{
			if (_out[_i].Contains("1595"))
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

	TObjectPtr<UPaperTileMap> _map = CreateInstanceTileMap(mapActor->GetRenderComponent());
	TObjectPtr<UPaperTileMap> _collectMap = CreateInstanceTileMap(mapActor->GetCollectMapComponent());
	CreateTileMapFromData(_map);
	GenerateBackgroundLayer(_map);
	CreateCollectibleMap(_collectMap);
	mapActor->GetRenderComponent()->RebuildCollision();
	mapActor->GetCollectMapComponent()->RebuildCollision();
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
	return TObjectPtr<UPaperTileMap>();
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

// Called every frame
void ALevelMapBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

