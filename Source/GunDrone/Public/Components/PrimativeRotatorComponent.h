// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Data/StaticsLibrary.h"
#include "PrimativeRotatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUNDRONE_API UPrimativeRotatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrimativeRotatorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<class UPrimitiveComponent*> RotatingComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<FRotator> RotationSpeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<float> InterpolationSpeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		bool bRotateComponentToActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<class UPrimitiveComponent*> TargetRotationComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<float> TargetedInterpolationSpeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<TEnumAsByte<EEntityTargetRotation::Type>> TargetedRotationTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<bool> bOnlyRotateComponentYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TArray<FVector> LastForwardVectors;






protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateRotatingComponents(float Delta);

	UFUNCTION(BlueprintCallable)
		void SetRotatingComponent(UPrimitiveComponent* InComponent, 
			FRotator InRotationSpeed, float InInterpolationSpeed);

	UFUNCTION(BlueprintCallable)
		void RemoveRotatingComponent(UPrimitiveComponent* InComponent);

	UFUNCTION(BlueprintCallable)
		void SetTargetedRotation(UPrimitiveComponent* InComponent,
			AActor* InActor, float InInterpolationSpeed, 
			EEntityTargetRotation::Type InType, bool bOnlyUpdateYaw = false);
};
