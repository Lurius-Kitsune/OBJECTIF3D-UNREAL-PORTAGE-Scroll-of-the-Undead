// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectComponent.generated.h"

class APaperPlayer;
class UPaperTileMapComponent;
struct FPaperTileMetadata;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REVISIONP2_API UCollectComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APaperPlayer> owner;

public:	
	// Sets default values for this component's properties
	UCollectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Collect(UPaperTileMapComponent* _tileMap, const FVector2D& _playerTilePos);
};
