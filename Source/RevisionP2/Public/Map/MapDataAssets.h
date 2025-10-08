// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperTileSet.h"
#include "MapDataAssets.generated.h"

/**
 * 
 */

class UPaperTileSet;

UCLASS()
class REVISIONP2_API UMapDataAssets : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TObjectPtr<USoundBase> music = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TObjectPtr<UTexture2D> backgroundImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FVector2D mapSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	float gravity = -980.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FVector2D friction = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map", meta = (MultiLine = true))
	TObjectPtr<UPaperTileSet> tileSet = LoadObject<UPaperTileSet>(nullptr, TEXT("/Game/BP/TileSheet.TileSheet"));
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map", meta = (MultiLine = true))
	FString mapDataString = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map", meta = (MultiLine = true))
	FString mapBackgroundDataString = "";
};
