// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/MapDataAssets.h"
#include "LevelMapBuilder.generated.h"

class UPaperTileMap;
class UPaperTileSet;
class ALevelMapActor;
class APaperEnemy;
class APaperCharacterActor;
enum class ELevelType : uint8;
struct FPaperTileInfo;

class UPaperTileMapComponent;
class UAudioComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Data")
	TMap<EEnemyType, TSubclassOf<APaperEnemy>> enemyBlueprints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Data")
	TSubclassOf<ALevelMapActor> actorBlueprint;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UBillboardComponent> icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> audioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Data")
	TMap<ELevelType, TObjectPtr<UMapDataAssets>> mapDataAssets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Debug")
	TObjectPtr<UMapDataAssets> data;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TArray<FString> collectibleList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TArray<FString> tile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TArray<FString> player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TArray<FString> enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TArray<FString> collectible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TObjectPtr<ALevelMapActor> mapActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Debug")
	TObjectPtr<AActor> playerActor;
	
public:	
	// Sets default values for this actor's properties
	ALevelMapBuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupVariables();
	void ParseAllData();


	virtual TArray<FString> ParseStringFromData(const FString& _data);

	virtual TObjectPtr<UPaperTileMap> CreateInstanceTileMap(TObjectPtr<UPaperTileMapComponent> _component);
	virtual TObjectPtr<UPaperTileMap> CreateTileMapFromData(TObjectPtr<UPaperTileMap> _map);
	virtual TObjectPtr<UPaperTileMap> CreateCollectibleMap(TObjectPtr<UPaperTileMap> _map);

#pragma region Generator
	virtual void GenerateBackgroundLayer(TObjectPtr<UPaperTileMap> _map);
	void GenerateCell(const FString& _data, const int& _layer, TObjectPtr<UPaperTileMap> _map);
	FPaperTileInfo GenerateTileInfo(const int& _index, const TObjectPtr<UPaperTileSet>& _tileSet);
#pragma endregion

#pragma region Placer
	void PlaceMusic();
	void PlaceMap(const TObjectPtr<UPaperTileMap>& _map);
	virtual void PlacePlayer(const TObjectPtr<UPaperTileMap>& _map);
	virtual void PlaceEntity(TObjectPtr<APaperCharacterActor> _actor, const TObjectPtr<UPaperTileMap>& _map, const FVector2D& _cords);
	virtual void PlaceEnemyInMap(const TObjectPtr<UPaperTileMap>& _map);
#pragma endregion


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
