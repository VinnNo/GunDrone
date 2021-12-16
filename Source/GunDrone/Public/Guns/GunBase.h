// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Public/Character/CharacterBase.h"
#include "GunBase.generated.h"

UENUM(BlueprintType)
namespace EGunHand
{
	enum Type
	{
		None    UMETA(DisplayName = "None"),
		Right   UMETA(DisplayName = "Right"),
		Left    UMETA(DisplayName = "Left"),
	};
}

UCLASS()
class GUNDRONE_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ArrowShot;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float Accuracy = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float Spread = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float EnergyDrainRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		bool bIsFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float BonusVelocityMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		int BounceCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float Gravity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		TEnumAsByte<EGunHand::Type> GunHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsRecoil = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsFired = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float RecoilSpeedMultiplier = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float RecoilRecoverySpeedMultiplier = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float RecoilSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float RecoilRecoverySpeed = 0.5f;

	bool bWantsToFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStats)
		class ACharacterBase* CharacterOwner = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		FRotator RecoilRotation = FRotator(13.0f, 0.000000f, 0.000000f);

	//(Pitch=13.120748,Yaw=0.000000,Roll=0.000000)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullets)
		TSubclassOf<class ABulletBase> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullets)
		class ABulletBase* Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullets)
		TArray<UPrimitiveComponent*> ShotPositions;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Targeting)
		FVector TargetLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool bDrawAimTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool bIsDebug = true;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AimTrace(FVector End);

	UFUNCTION(BlueprintCallable)
		void SetFire(FVector End);

	UFUNCTION(BlueprintCallable)
		void StopFire();

	void SpawnShot(FVector End);

	void UpdateGunRotation(float Delta);

};
