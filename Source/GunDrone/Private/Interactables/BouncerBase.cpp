// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/BouncerBase.h"
#include "Components/SceneComponent.h"
#include "../../Public/Character/CharacterBase.h"

// Sets default values
ABouncerBase::ABouncerBase()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABouncerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABouncerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABouncerBase::Bounce(ACharacterBase* InCharacter)
{
	if (InCharacter != nullptr)
	{
		LaunchDirection = GetActorForwardVector();
		InCharacter->LaunchCharacter(LaunchDirection * LaunchStrength, true, true);
		InCharacter->JumpCountCurrent = (InCharacter->JumpCountMax + InCharacter->JumpCountBoosted);
		InCharacter->OnJumpCountCurrentChanged(InCharacter->GetRemainingJumps());
	}
}

