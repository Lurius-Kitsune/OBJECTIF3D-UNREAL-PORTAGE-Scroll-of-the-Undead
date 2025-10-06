// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/EntityManager.h"
#include "Entity/BaseEntity.h"
#include "Kismet/KismetSystemLibrary.h"

void UEntityManager::Add(const TObjectPtr<ABaseEntity>& _entity)
{
	if (IsPresent(_entity)) return;
	entities.Add(_entity);
}

bool UEntityManager::Remove(const TObjectPtr<ABaseEntity>& _entity)
{
	if (!IsPresent(_entity)) return false;
	entities.Remove(_entity);
	return true;
}

bool UEntityManager::RemoveAndDestroy(const TObjectPtr<ABaseEntity>& _entity)
{
	if (!IsPresent(_entity)) return false;
	Remove(_entity);
	_entity->Destroy();
	return true;
}

bool UEntityManager::Purge()
{
	if (entities.Num() <= 0) return false;
	entities.Empty();
	return true;
}

bool UEntityManager::IsPresent(const TObjectPtr<ABaseEntity>& _entity)
{
	return entities.Find(_entity) != INDEX_NONE;
}
