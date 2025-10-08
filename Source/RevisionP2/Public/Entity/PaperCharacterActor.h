// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/BaseEntity.h"
#include "Enum/Entity.h"
#include "PaperCharacterActor.generated.h"

/**
 * 
 */

class UPaperFlipbookComponent;
class UBoxComponent;
struct FInputActionValue;
class UPaperZDAnimationComponent;

UCLASS()
class REVISIONP2_API APaperCharacterActor : public ABaseEntity
{
	GENERATED_BODY()
	
protected:

#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovement, const FVector2D&, direction);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnMovement onMovement;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnDeath onDeath;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectionChanged, const EEntityDirection&, direction);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnDirectionChanged onDirectionChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceived, const int&, damageValue);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageReceived onDamageReceived;
#pragma endregion


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> sprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperZDAnimationComponent> animation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> attackHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug|Entity")
	EEntityDirection direction = EEntityDirection::Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	float jumpVelocity = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	int hitPoints = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Entity")
	int currentHitPoints = 200;


public:
	// Sets default values for this pawn's properties
	APaperCharacterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Getters
	UFUNCTION(BlueprintPure)
	FORCEINLINE EEntityDirection GetDirection() const { return direction; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetHitpoints() const { return hitPoints; }
#pragma endregion

#pragma region Setters
	UFUNCTION(BlueprintCallable)
	void SetDirection(const EEntityDirection& _dir);

	UFUNCTION(BlueprintCallable)
	void ToogleAttackHitbox(const bool _enable);
#pragma endregion

#pragma region Character Action
	UFUNCTION(BlueprintCallable)
	void Move(const EEntityDirection& _value);
	void Move(const FInputActionValue& _value);
	UFUNCTION(BlueprintCallable)
	void Jump();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void GetHurt(const int& _damage);

#pragma endregion



};
