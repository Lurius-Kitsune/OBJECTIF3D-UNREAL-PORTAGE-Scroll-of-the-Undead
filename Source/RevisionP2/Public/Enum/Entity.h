// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EEntityType : uint8
{
	Base,
	Enemy,
	Player
};


UENUM(BlueprintType)
enum class EEntityState : uint8
{
	Idle,
	Walking,
	Jumping,
	Attacking,
	Hurt,
	Dying
};

UENUM(BlueprintType)
enum class EEntityDirection : uint8
{
	Left,
	Right
};
