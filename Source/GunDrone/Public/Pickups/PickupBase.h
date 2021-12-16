// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UENUM(BlueprintType)
namespace EPickupType
{
	enum Type
	{
		Health   UMETA(DisplayName = "HealthUp"),
		Energy   UMETA(DisplayName = "Energy"),
		Jump     UMETA(DisplayName = "Jump"),
		Heal     UMETA(DisplayName = "Heal"),
		Charge   UMETA(DisplayName = "Charge"),
		Data     UMETA(DisplayName = "Data"),
	};
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOverlappedPlayer,
AActor*, OutActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectedPlayer,
AActor*, OutActor);

UCLASS()
class GUNDRONE_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	UPROPERTY(BlueprintAssignable)
		FOverlappedPlayer OnPlayerOverlapped;

	UPROPERTY(BlueprintAssignable)
		FDetectedPlayer OnPlayerDetected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<EPickupType::Type> PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float PickupValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		bool bHasMagnetism = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		AActor* MagnetTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		bool bMagnetismIsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		AActor* TargetActor;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetected(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SetPlayerValues(ACharacterBase* InCharacter);

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdatedPlayer();

	UFUNCTION(BlueprintCallable)
		void SetMagnitismActive(AActor* InActor, bool bActive);

};
