// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelMapBuilder.generated.h"

class UMapDataAssets;
class UPaperTileMap;
class UPaperTileSet;
class UPaperTileMapComponent;
class ALevelMapActor;
struct FPaperTileInfo;

UCLASS()
class REVISIONP2_API ALevelMapBuilder : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TSoftClassPtr<ALevelMapActor> actorBlueprint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TObjectPtr<UMapDataAssets> data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<FString> collectibleList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TArray<FString> tile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TArray<FString> player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TArray<FString> enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TArray<FString> collectible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TObjectPtr<ALevelMapActor> mapActor;
	
public:	
	// Sets default values for this actor's properties
	ALevelMapBuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual TArray<FString> ParseStringFromData(const FString& _data);
	virtual TObjectPtr<UPaperTileMap> CreateInstanceTileMap(TObjectPtr<UPaperTileMapComponent> _component);
	virtual TObjectPtr<UPaperTileMap> CreateTileMapFromData(TObjectPtr<UPaperTileMap> _map);
	virtual TObjectPtr<UPaperTileMap> CreateCollectibleMap(TObjectPtr<UPaperTileMap> _map);
	virtual void GenerateBackgroundLayer(TObjectPtr<UPaperTileMap> _map);
	void GenerateCell(const FString& _data, const int& _layer, TObjectPtr<UPaperTileMap> _map);
	FPaperTileInfo GenerateTileInfo(const int& _index, const TObjectPtr<UPaperTileSet>& _tileSet);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
