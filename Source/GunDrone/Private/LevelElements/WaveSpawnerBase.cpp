// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/WaveSpawnerBase.h"
#include "Components/ChildActorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
AWaveSpawnerBase::AWaveSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveSpawnerBase::SetupWaves()
{
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);


	TArray<UActorComponent*> ChildActorComponenents;
	TSet<UActorComponent> ActorComponents;

	//GetComponents(ActorComponents);

	ChildActorComponenents = GetComponentsByClass(UChildActorComponent::StaticClass());




	if (ChildActorComponenents.Num() > 0)
	{
		for (UActorComponent* ChildComponent : ChildActorComponenents)
		{
			TArray<FName> TagNames;
			TagNames = ChildComponent->ComponentTags;

			for (FName TagName : TagNames)
			{
				// The TagString pointer needs to be initialized with SOME value
				FString TagNonPointer = TEXT("");

				FString* TagString = &TagNonPointer;
				TagName.ToString(*TagString);

				if (TagString->IsNumeric())
				{
					
					int WaveNumber = UKismetStringLibrary::Conv_StringToInt(*TagString);

					UChildActorComponent* Child = Cast<UChildActorComponent>(ChildComponent);
					if (IsValid(Child))
					{
						WaveChildNumbers.Add(WaveNumber);
						WaveChildClasses.Add(Child->GetChildActorClass());
						WaveChildTransforms.Add(Child->GetComponentTransform());
					}

				}
			}
		}

		OnChildActorsAssigned();
	}
}


void AWaveSpawnerBase::SpawnWaves()
{
	if (WaveChildNumbers.Num() > 0)
	{
		for (int Itr = WaveChildNumbers.Num() - 1; Itr > 0; --Itr)
		{
			if (WaveChildNumbers[Itr] == WaveCurrent)
			{
				OnWaveSpawnActor(WaveChildClasses[Itr], WaveChildTransforms[Itr]);

				//WaveChildNumbers.Remove(WaveChildNumbers[Itr]);
				//WaveChildClasses.Remove(WaveChildClasses[Itr]);
				//WaveChildTransforms.Remove(WaveChildTransforms[Itr]);
			}
		}
	}
}


