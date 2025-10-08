// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/MapDataAssets.h"
#include "LevelMapBuilder.generated.h"

class UPaperTileMap;
class UPaperTileSet;
class UPaperTileMapComponent;
class UAudioComponent;
class ALevelMapActor;
class APaperEnemy;
class APaperCharacterActor;
enum class ELevelType : uint8;
struct FPaperTileInfo;

UENUM(BlueprintType)
enum class EEnemyType: uint8
{
	Skeleton,
	Goblin,
	Mushroom,
	Winged,
	None
};

UCLASS()
class REVISIONP2_API ALevelMapBuilder : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TMap<EEnemyType, TSubclassOf<APaperEnemy>> enemyBlueprints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TSubclassOf<ALevelMapActor> actorBlueprint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TMap<ELevelType, TObjectPtr<UMapDataAssets>> mapDataAssets;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TObjectPtr<AActor> playerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> audioComponent;
	
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


	void PlaceMap(const TObjectPtr<UPaperTileMap>& _map);
	virtual void PlacePlayer(const TObjectPtr<UPaperTileMap>& _map);
	virtual void PlaceEntity(TObjectPtr<APaperCharacterActor> _actor, const TObjectPtr<UPaperTileMap>& _map, const FVector2D& _cords);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE ALevelMapActor* GetMapActor() const { return mapActor; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetPlayerActor() const { return playerActor; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE UMapDataAssets* GetMapData() const { return data; }

	UFUNCTION(BlueprintPure) 
	FORCEINLINE float GetGravity() const { return data ? data->gravity : -980.0f; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetFriction() const { return data ? data->friction : FVector2D::ZeroVector; }
};
