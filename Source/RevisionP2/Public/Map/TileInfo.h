// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTileInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D friction = FVector2D(0.0f);

	bool operator==(const FTileInfo& other) const = default;

	FTileInfo() = default;

	FTileInfo(int id, const FString& name, const FVector2D& friction)
		: id(id), name(name), friction(friction)
	{
	}
};
