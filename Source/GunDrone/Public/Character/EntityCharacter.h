// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/StaticsLibrary.h"
#include "EntityCharacter.generated.h"


UCLASS()
class GUNDRONE_API AEntityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntityCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedPatrol = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedChase = 220.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedEngage = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float ChaseDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SightDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		class ACharacterBase* TargetCharacter = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		TEnumAsByte<EEntityStates::Type> EntityAIState = EEntityStates::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bJumpAtRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float JumpTimeCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FVector2D JumpTimeMaxRange = FVector2D(3.0f, 6.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float JumpTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bCanJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bUpdateTargetToPlayer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bTraceFromComponent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UPrimitiveComponent* TraceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		AActor* TraceTarget;






protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateEntityStates(float Delta);

	UFUNCTION(BlueprintImplementableEvent)
		void OnMovementCompleted(EEntityStates::Type OutState);

	UFUNCTION(BlueprintCallable)
	void SetEntityState(TEnumAsByte<EEntityStates::Type> InState);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEntityStateChanged(EEntityStates::Type OutState);

	UFUNCTION(BlueprintImplementableEvent)
		void OnPlayerTargetAcquired();

	UFUNCTION(BlueprintCallable)
		bool TraceForPlayer(bool bDrawDebug = false);
};
