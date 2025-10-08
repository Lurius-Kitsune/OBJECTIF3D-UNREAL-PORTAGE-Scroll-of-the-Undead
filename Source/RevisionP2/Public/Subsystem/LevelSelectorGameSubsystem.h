// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSelectorGameSubsystem.generated.h"

/**
 * 
 */

UENUM()
enum class ELevelType : uint8
{
	None,

	Tutorial,
	Level1,
	Level2
};

UCLASS()
class REVISIONP2_API ULevelSelectorGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	ELevelType currentLevel = ELevelType::None;

public:
	UFUNCTION(BlueprintPure)
	ELevelType GetCurrentLevel() const { return currentLevel; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(ELevelType newLevel) { currentLevel = newLevel; }
};
