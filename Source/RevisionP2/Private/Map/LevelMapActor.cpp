// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelMapActor.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"

ALevelMapActor::ALevelMapActor()
{
	collectMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("CollectMapComponent"));
	collectMapComponent->SetupAttachment(RootComponent);
}

FVector2D ALevelMapActor::GetMapSize() const { return FVector2D(GetRenderComponent()->TileMap->MapWidth, GetRenderComponent()->TileMap->MapHeight); }

FVector2D ALevelMapActor::GetRealMapSize() const { return GetMapSize() * FVector2D(GetRenderComponent()->TileMap->TileWidth - 0.5, GetRenderComponent()->TileMap->TileHeight); }
