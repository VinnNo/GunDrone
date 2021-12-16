// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawnerBase.generated.h"

UCLASS()
class GUNDRONE_API AWaveSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawnerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
		TArray<TSubclassOf<class AActor>> WaveChildClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
		TArray<FTransform> WaveChildTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
		TArray<int> WaveChildNumbers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
		int WaveCurrent = 0;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetupWaves();

	UFUNCTION(BlueprintCallable)
		void SpawnWaves();

	UFUNCTION(BlueprintImplementableEvent)
		void OnWaveSpawnActor(TSubclassOf<AActor> OutClass, FTransform OutTransform);

	UFUNCTION(BlueprintImplementableEvent)
		void OnChildActorsAssigned();

};
