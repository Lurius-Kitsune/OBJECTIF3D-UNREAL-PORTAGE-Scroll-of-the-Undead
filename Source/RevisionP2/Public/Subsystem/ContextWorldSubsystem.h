// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ContextWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REVISIONP2_API UContextWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	// EVENT
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinsUpdate);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnCoinsUpdate onCoinsUpdate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBooksUpdate);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnBooksUpdate onBooksUpdate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStonesUpdate);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnStonesUpdate onStonesUpdate;

	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	int playerCoins = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	int playerBooks = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	int playerStones = 0;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetPlayerCoins() const { return playerCoins; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetPlayerBooks() const { return playerBooks; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetPlayerStones() const { return playerStones; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerCoins(const int& _value);
	UFUNCTION(BlueprintCallable)
	void SetPlayerBooks(const int& _value);
	UFUNCTION(BlueprintCallable)
	void SetPlayerStones(const int& _value);

	UFUNCTION(BlueprintCallable)
	void AddCoins(const int& _value);
	UFUNCTION(BlueprintCallable)
	void AddBooks(const int& _value);
	UFUNCTION(BlueprintCallable)
	void AddStones(const int& _value);
};
