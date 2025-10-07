// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelMapActor.h"
#include "PaperTileMapComponent.h"

ALevelMapActor::ALevelMapActor()
{
	collectMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("CollectMapComponent"));
	collectMapComponent->SetupAttachment(RootComponent);
}
