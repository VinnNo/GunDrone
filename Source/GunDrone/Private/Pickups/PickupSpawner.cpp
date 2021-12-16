// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupSpawner.h"

#include "../Public/Pickups/PickupBase.h"

// Sets default values
APickupSpawner::APickupSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupSpawner::GeneratePickups(int InIterations)
{
	if (InIterations == 0)
	{
		return;
	}

	if (PickupRarities.Num() == 0)
	{
		return;
	}

	if (PickupsList.Num() == 0)
	{
		return;
	}

	TArray<TSubclassOf<class APickupBase>> Pickups;

	for (int Itr = 0; Itr < InIterations; ++Itr)
	{
		float Chance = FMath::RandRange(1.0f, 100.0f);
		for (int Jtr = PickupRarities.Num() - 1; Jtr > 0; --Jtr)
		{
			if (PickupRarities[Jtr] < Chance)
			{
				continue;
			}
			else
			{
				Pickups.Add(PickupsList[Jtr]);
				float SecondChance = FMath::RandRange(1.0f, 100.0f);
				if (SecondChance < Chance)
				{
					Itr += 1;
					break;
				}
			}
		}
	}

	if (Pickups.Num() > 0)
	{
		for (int Itr = 0; Itr < Pickups.Num(); ++Itr)
		{
			OnPickupSpawnSucceeded(Pickups[Itr]);
		}
	}
}

FVector APickupSpawner::GetRandomizedSpawnRange()
{
	FVector Start = GetActorLocation();
	FVector End;

	FVector RNG = RandomizedPickupSpawnRange;
	End.X = FMath::RandRange(-RNG.X, RNG.X);
	End.Y = FMath::RandRange(-RNG.Y, RNG.Y);
	End.Z = FMath::RandRange(-RNG.Z, RNG.Z);

	FVector Out = Start + End;

	return Out;
}

