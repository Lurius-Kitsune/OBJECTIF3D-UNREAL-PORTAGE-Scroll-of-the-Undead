// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/EntityManager.h"
#include "Entity/BaseEntity.h"

void UEntityManager::Add(const TObjectPtr<ABaseEntity>& _entity)
{
	if (IsPresent(_entity)) return;
	entities.Add(_entity);
}

bool UEntityManager::IsPresent(const TObjectPtr<ABaseEntity>& _entity)
{
	return entities.Find(_entity) != INDEX_NONE;
}
