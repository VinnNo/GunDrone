// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawner.generated.h"

UCLASS()
class GUNDRONE_API APickupSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Types")
		TArray<TSubclassOf<class APickupBase>> PickupsList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Types")
		TArray<float> PickupRarities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Types")
		FVector RandomizedPickupSpawnRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup|Types")
		int IterationsOfPickupList = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GeneratePickups(int InIterations);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetRandomizedSpawnRange();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPickupSpawnSucceeded(TSubclassOf<class APickupBase> PickupClass);

};
