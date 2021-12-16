// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUNDRONE_API UEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyComponent();

	UPROPERTY(BlueprintAssignable)
		FOnEnemyHit OnEnemyHit;

	UPROPERTY(BlueprintAssignable)
		FOnEnemyDeath OnEnemyDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentReference)
		class UPrimitiveComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float HealthMax = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float HealthCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		bool bIsHitflash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		FVector HitFlashStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		FVector HitFlashEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		float HitFlashSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		UMaterialInstanceDynamic* HitFlashMaterial;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitBindings();
		
	UFUNCTION()
		void OnHitBinding(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBinding(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	void TakeDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
		void HitFlashUpdate(float Delta);

	UFUNCTION(BlueprintCallable)
		void SetHitFlash(bool bSetActive, UPrimitiveComponent* InComp, FVector Start, FVector End);
};
