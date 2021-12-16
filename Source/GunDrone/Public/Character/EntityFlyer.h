// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/StaticsLibrary.h"
#include "EntityFlyer.generated.h"

UCLASS()
class GUNDRONE_API AEntityFlyer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntityFlyer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* DebugArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* MovementComponenent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		TEnumAsByte<EEntityStates::Type> EntityAIState = EEntityStates::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedPatrol = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedChase = 220.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float SpeedEngage = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float ChaseDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		class ACharacterBase* TargetCharacter = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsBasic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		int StrafeDirection = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FRotator CurrentRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FRotator TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FRotator DefinedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float RotationSpeed = 20.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float EngagementTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float EngagementTimeMax = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FVector2D EngagementRange = FVector2D(2.0f, 4.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FVector EngagementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float ActionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float ActionTimeMax = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bUseActionTimer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bUpdateTargetToPlayer = true;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		TEnumAsByte<EEntityEngagement::Type> EngagementType = EEntityEngagement::Strafe;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateEntityStates(float Delta);

	UFUNCTION(BlueprintCallable)
	void SetEntityState(TEnumAsByte<EEntityStates::Type> InState);

	UFUNCTION(BlueprintCallable)
		void SetEntityEngagement(TEnumAsByte<EEntityEngagement::Type> InEngagement);

	void SetTargetRotation(FVector InTargetLocation);

	FRotator UpdateRotation(float Delta);

	FVector RandomizeEngagementDirection();

	void UpdateActionTime(float Delta);

	UFUNCTION(BlueprintCallable)
	bool TraceForPlayer(bool bDrawDebug = false);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEntityStateChanged(EEntityStates::Type OutState);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEntityEngagementChanged(EEntityEngagement::Type OutEngagement);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActionTime();
};
