// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CharacterBase.generated.h"

UCLASS()
class GUNDRONE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* HeadMount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arms, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* RightArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arms, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* LeftArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPSCamera;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FVector HitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guns)
		TSubclassOf<class AGunBase> GunRightClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guns)
		TSubclassOf<class AGunBase> GunLeftClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guns)
		class AGunBase* GunRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guns)
		class AGunBase* GunLeft;

	// Jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Jumping")
		int JumpCountCurrent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Jumping")
		int JumpCountMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Jumping")
		int JumpCountBoosted = 0;


	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
		float HealthMax = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
		float HealthBoosted = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
		float HealthCurrent = 0.0f;

	// Data - EXP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|DataEXP")
		int DataMax = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|DataEXP")
		int DataCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|DataEXP")
		float DataScalar = 0.675f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|DataEXP")
		int DataPoints;

	// Energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float EnergyMax = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float EnergyBoosted = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float EnergyLeftCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float EnergyRightCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		bool bGunRightRecharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		bool bGunLeftRecharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float GunEnergyRechargeRate = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float EnergyRechargeDelay = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Energy")
		float KineticEnergyRechargeRate = 0.5;

	FTimerHandle RightGunDelayHandle;
	FTimerHandle LeftGunDelayHandle;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverRangeIdle = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverRangeWalking = 18.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverSpeedIdle = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverSpeedWalking = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverRange = HoverSpeedWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float HoverSpeed = HoverSpeedIdle;

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InputMoveForward(float Value);
	void InputMoveRight(float Value);

	void UpdateKineticEnergy();

	void InputJumpPressed();
	void InputJumpReleased();

	void InputFireRightPressed();
	void InputFireRightReleased();

	void InputFireLeftPressed();
	void InputFireLeftReleased();

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void AimTrace();

	UFUNCTION(BlueprintCallable)
	void EquipGun(bool bIsRight);

	void SpawnGun(TSubclassOf<class AGunBase> InGun, FVector InLocation, FRotator InRotation);

	void UpdateCameraRotation(float Delta);

	void SetJumpCounts(float Value);

	void DoLaunch(FVector LaunchVelocity);

	UFUNCTION()
		void OnOverlapBinding(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void HealPlayer(float Value);

	UFUNCTION(BlueprintCallable)
	void DamagePlayer(float Value, bool bApplyForce, FVector Force);

	UFUNCTION(BlueprintCallable)
		void UpgradeHealth(float Value);

	UFUNCTION(BlueprintCallable)
		void UpgradeEnergy(float Value);

	UFUNCTION(BlueprintCallable)
		void UpgradeJumps(int Value);

	UFUNCTION(BlueprintCallable)
		void HealEnergy(float Value);

	UFUNCTION(BlueprintCallable)
		void AddPlayerData(int Value);

	UFUNCTION(BlueprintCallable)
		void DrainGunEnergy(float DrainCost, bool bIsRight = true);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stats")
		void OnHealthChanged(bool bIsDamaged, float Percent);

	UFUNCTION(BlueprintCallable)
		void RechargeRightGunEnergy(float Delta);

	UFUNCTION(BlueprintCallable)
		void RechargeLeftGunEnergy(float Delta);

	UFUNCTION(BlueprintCallable)
		void SetRechargeTimer(bool bIsRight);


	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDeath();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPlayerDamaged();

	UFUNCTION(BlueprintImplementableEvent)
		void OnJumpCountMaxChanged(int MaxJumps);

	UFUNCTION(BlueprintImplementableEvent)
		void OnJumpCountCurrentChanged(int RemainingJumps);

	UFUNCTION(BlueprintImplementableEvent)
		void OnRightEnergyChanged(float EnergyPercent);

	UFUNCTION(BlueprintImplementableEvent)
		void OnLeftEnergyChanged(float EnergyPercent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		int GetRemainingJumps();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		int GetMaxJumps();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetHealthAsPercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetRightEnergyAsPercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetLeftEnergyAsPercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetDataAsPercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		float GetMaxEnergy();

	UFUNCTION(BlueprintCallable)
		void SavePlayerStats();

	UFUNCTION(BlueprintCallable)
		void LoadPlayerStats();

	//UFUNCTION()
	//	void OnPlayerStatsLoaded(const FString& SlotName, const int32 UserIndex,
	//		USaveGame* LoadedGameData);

};
