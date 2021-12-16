// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../../Public/Character/CharacterBase.h"

#include "Kismet/KismetMathLibrary.h"

#include "Engine.h"

// Sets default values
APickupBase::APickupBase()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.25f;


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlap);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnDetected);
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasMagnetism)
	{
		if (bMagnetismIsActive)
		{
			if (!IsValid(MagnetTarget))
			{
				return;
			}
			
			FVector Start = GetActorLocation();
			FVector End = MagnetTarget->GetActorLocation();

			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

			FVector Direction = TargetRotation.Vector();
			float Speed = ProjectileMovement->HomingAccelerationMagnitude;

			ProjectileMovement->Velocity = Direction * Speed;
		}
	}

}

void APickupBase::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, OtherActor->GetName());
	if (OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		OnPlayerOverlapped.Broadcast(OtherActor);
		ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
		if (IsValid(Character))
		{
			SetPlayerValues(Character);
		}
	}
}

void APickupBase::OnDetected(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		OnPlayerOverlapped.Broadcast(OtherActor);
	}
}

void APickupBase::SetPlayerValues(ACharacterBase* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}
	switch (PickupType)
	{
		case EPickupType::Health:
		{
			InCharacter->UpgradeHealth(PickupValue);
			break;
		}
		case EPickupType::Energy:
		{
			InCharacter->UpgradeEnergy(PickupValue);
			break;
		}
		case EPickupType::Jump:
		{
			//InCharacter->SetJumpCounts(PickupValue);
			int Value = round(PickupValue);
			InCharacter->UpgradeJumps(Value);
			break;
		}
		case EPickupType::Heal:
		{
			InCharacter->HealPlayer(PickupValue);
			break;
		}
		case EPickupType::Charge:
		{
			InCharacter->HealEnergy(PickupValue);
			break;
		}
		case EPickupType::Data:
		{
			int Value = round(PickupValue);
			InCharacter->AddPlayerData(Value);
			break;
		}
	}

	OnUpdatedPlayer();
}

void APickupBase::SetMagnitismActive(AActor* InActor, bool bActive)
{
	if (!bHasMagnetism && !IsValid(InActor))
	{
		return;
	}

	bMagnetismIsActive = bActive;

	if (bActive)
	{
		//ProjectileMovement->bIsHomingProjectile = true;
		//ProjectileMovement->HomingTargetComponent = InActor->GetRootComponent();
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 0.0f;

		MagnetTarget = InActor;

		FVector Start = GetActorLocation();
		FVector End = InActor->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		FVector Direction = TargetRotation.Vector();
		float Speed = ProjectileMovement->MaxSpeed;

		ProjectileMovement->Velocity = Direction * Speed;
	}

}