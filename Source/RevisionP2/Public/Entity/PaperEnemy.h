// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/PaperCharacterActor.h"
#include "PaperEnemy.generated.h"

/**
 * 
 */
UCLASS()
class REVISIONP2_API APaperEnemy : public APaperCharacterActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D destination;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool hasDestination = false;

public:
	// Sets default values for this pawn's properties
	APaperEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnEntityCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);
};
