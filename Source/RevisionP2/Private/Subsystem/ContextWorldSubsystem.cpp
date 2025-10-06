// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ContextWorldSubsystem.h"

void UContextWorldSubsystem::SetPlayerCoins(const int& _value)
{
	playerCoins = _value;
	onCoinsUpdate.Broadcast();
}

void UContextWorldSubsystem::SetPlayerBooks(const int& _value)
{
	playerBooks = _value;
	onBooksUpdate.Broadcast();
}

void UContextWorldSubsystem::SetPlayerStones(const int& _value)
{
	playerStones = _value;
	onStonesUpdate.Broadcast();
}

void UContextWorldSubsystem::AddCoins(const int& _value)
{
	playerCoins += _value;
	onCoinsUpdate.Broadcast();
}

void UContextWorldSubsystem::AddBooks(const int& _value)
{
	playerBooks += _value;
	onBooksUpdate.Broadcast();
}

void UContextWorldSubsystem::AddStones(const int& _value)
{
	playerStones += _value;
	onStonesUpdate.Broadcast();
}
