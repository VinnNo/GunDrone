// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

UENUM(BlueprintType)
namespace EBulletType
{
	enum Type
	{
		None    UMETA(DisplayName = "None"),
		Player  UMETA(DisplayName = "Player"),
		Enemy   UMETA(DisplayName = "Enemy"),
	};
}

UCLASS()
class GUNDRONE_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow;

	FTimerHandle LifeTimeHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float LifeTime = 20.0f;

	UPROPERTY()
		float LifeTimeCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		bool bUseLifeTime = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float Damage = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int BounceCountMax = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int BounceCountCurrent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		bool bWaitOnDestroy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		TEnumAsByte<EBulletType::Type> BulletType = EBulletType::Player;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitSpeed(float Value);

	UFUNCTION(BlueprintCallable)
		void InitStats(float InSpeed, int InBounce, 
			float InGravity = 0.0f, float InDamage = 0.1f);

	UFUNCTION(BlueprintImplementableEvent)
		void OnLifeTimeUp();

	void UpdateTimer(float Delta);

	UFUNCTION()
		void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void OnDeath();

	UFUNCTION(BlueprintCallable)
		void BulletDeath();


	FORCEINLINE class UProjectileMovementComponent* GetMovement() const { return ProjectileMovement; }

};
