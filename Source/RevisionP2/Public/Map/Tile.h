// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D friction = FVector2D(0.0f);

	FTileInfo()
	{
		id = 0;
		name = "Default";
		friction = FVector2D(1.0f, 1.0f);
	}
};


UCLASS()
class REVISIONP2_API ATile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTileInfo tileInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isWarp = false;

public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
