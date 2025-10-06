// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EntityManager.generated.h"

/**
 * 
 */

class ABaseEntity;

UCLASS()
class REVISIONP2_API UEntityManager : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ABaseEntity>> entities;

public:
	void Add(const TObjectPtr<ABaseEntity>& _entity);
	bool Remove(const TObjectPtr<ABaseEntity>& _entity);
	bool RemoveAndDestroy(const TObjectPtr<ABaseEntity>& _entity);

	bool Purge();
	bool IsPresent(const TObjectPtr<ABaseEntity>& _entity);
};
