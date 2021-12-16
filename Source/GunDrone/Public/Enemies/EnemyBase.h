// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"


UENUM(BlueprintType)
namespace EBaseCollisionShape
{
	enum Type
	{
		Box      UMETA(DisplayName = "Box"),
		Sphere   UMETA(DisplayName = "Sphere"),
		Capsule  UMETA(DisplayName = "Capsule"),
	};
}

UCLASS()
class GUNDRONE_API AEnemyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, 
		meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh,
		meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh,
		meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh,
		meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		TEnumAsByte<EBaseCollisionShape::Type> CollisionType;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitShapes();

	UFUNCTION()
	void BindCollisions();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyOverlap();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyHit();

};
