// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/BaseEntity.h"
#include "PaperCharacterActor.generated.h"

/**
 * 
 */

class UPaperFlipbookComponent;
class UBoxComponent;
struct FInputActionValue;

UCLASS()
class REVISIONP2_API APaperCharacterActor : public ABaseEntity
{
	GENERATED_BODY()
	
protected:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovement, const FVector2D&, direction);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMovement onMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbookComponent> sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEntityDirection direction = EEntityDirection::Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float jumpVelocity = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hitPoints = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentHitPoints = 200;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> attackHitbox;

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

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& _value);
	UFUNCTION(BlueprintCallable)
	void Jump();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void GetHurt(const int& _damage);

	//virtual void OnEntityCollision(EEntityBase l_collider, bool l_attack) = 0;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetHitpoints() const { return hitPoints; }

	UFUNCTION(BlueprintCallable)
	void ToogleAttackHitbox(const bool _enable);
};
