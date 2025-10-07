// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enum/Entity.h"
#include "BaseEntity.generated.h"

class UBoxComponent;
class UFlipbookComponent;
class UEntityManager;
class UContextWorldSubsystem;

UCLASS()
class REVISIONP2_API ABaseEntity : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name = "BaseEntity";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityType type = EEntityType::Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D position = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D positionOld = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D velocity = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D maxVelocity = FVector2D(1000.0f, 1000.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D speed = FVector2D(400.0f, 800.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D acceleration = FVector2D(2000.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D friction = FVector2D(800.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEntityState state = EEntityState::Idle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> hitbox;
	UPROPERTY(EditAnywhere)
	float zOffset = -2.0f;

	// DebugMode
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool debugMode = false;

	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<UAudioComponent> audioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> attackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> hurtSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> dieSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> pickupSound1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> pickupSound2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundBase> pickupSound3;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const EEntityState&, state);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnStateChanged onStateChanged;


	UPROPERTY()
	bool collidingOnX = false;

	// Subsystem
	UPROPERTY()
	TObjectPtr<UEntityManager> entityManager;

	UPROPERTY()
	TObjectPtr<UContextWorldSubsystem> contextManager;

	// TODO : DrawDebug
public:
	// Sets default values for this pawn's properties
	ABaseEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetPosition() const { return position; };
	//UFUNCTION(BlueprintPure)
	//FORCEINLINE FVector2D GetSize() const;
	UFUNCTION(BlueprintPure)
	EEntityState GetState() const { return state; }
	UFUNCTION(BlueprintPure)
	FString GetName() const { return name; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetID() const { return id; }
	UFUNCTION(BlueprintPure)
	EEntityType GetType() const { return type; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAudioComponent* GetAudioComponent() const { return audioComponent; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE USoundBase* GetPickupSound1() const { return pickupSound1; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE USoundBase* GetPickupSound2() const { return pickupSound2; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE USoundBase* GetPickupSound3() const { return pickupSound3; }


	void SetPosition(float _x, float _y);
	void SetPosition(const FVector2D& _pos);
	void SetSize(float _x, float _y);
	UFUNCTION(BlueprintCallable)
	void SetState(const EEntityState _state);

	void Move(const FVector2D& _movement);
	void AddVelocity(float _x, float _y);
	void Accelerate(float _x, float _y);
	void SetAcceleration(float _x, float _y);
	void ApplyFriction(float _x, float _y);

	UFUNCTION(BlueprintCallable)
	virtual void OnEntityCollision(UPrimitiveComponent* _me, AActor* _other, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);

};
