// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "LevelMapActor.generated.h"

class UPaperTileMapComponent;

/**
 * 
 */
UCLASS()
class REVISIONP2_API ALevelMapActor : public APaperTileMapActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UPaperTileMapComponent> collectMapComponent;

public:	
	ALevelMapActor();
	
#pragma region Getters
	UFUNCTION(BlueprintPure, Category = "Map")
	UPaperTileMapComponent* GetCollectMapComponent() const { return collectMapComponent; }

	UFUNCTION(BlueprintPure, Category = "Map")
	FVector2D GetMapSize() const;

	UFUNCTION(BlueprintPure, Category = "Map")
	FVector2D GetRealMapSize() const;
#pragma endregion

};
