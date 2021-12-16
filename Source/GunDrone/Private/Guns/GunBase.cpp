// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/GunBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "../../Public/Bullets/BulletBase.h"

#include "DrawDebugHelpers.h" 

// Sets default values
AGunBase::AGunBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMeshComponent->SetupAttachment(RootComponent);

	ArrowShot = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowShoot"));
	ArrowShot->SetupAttachment(GunMeshComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	
	RecoilSpeed = FireRate * RecoilSpeedMultiplier;
	RecoilRecoverySpeed = FireRate * RecoilRecoverySpeedMultiplier;

	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		APawn* Pawn = World->GetFirstPlayerController()->GetPawn();

		ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(Pawn);
		if (PlayerCharacter != nullptr)
		{
			CharacterOwner = PlayerCharacter;
		}
	}

}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGunRotation(DeltaTime);
}

void AGunBase::AimTrace(FVector End)
{
	float Distance = 10000.0f; // Just Some big number :/
	float LineThickness = 6.0f;
	uint8 Depth = 0;
	float LifeTime = 5.0f;

	FVector Start = ArrowShot->GetComponentLocation();

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FHitResult OutHit;

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	bool bUpdateTarget = false;

	if (bDrawAimTrace && bIsDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, LifeTime, Depth, LineThickness);
	}
}

void AGunBase::SetFire(FVector End)
{
	if (bIsFired || bIsRecoil)
	{
		bWantsToFire = true;
		return;
	}

	bool bCanShoot = false;

	if (IsValid(CharacterOwner))
	{
		End = CharacterOwner->HitLocation;

		switch (GunHand)
		{
			case EGunHand::None:
			{
				break;
			}
			case EGunHand::Right:
			{
				if (CharacterOwner->EnergyRightCurrent > 0.0f)
				{
					CharacterOwner->DrainGunEnergy(EnergyDrainRate, true);
					bCanShoot = true;
				}
				else
				{
					StopFire();
				}
				break;
			}
			case EGunHand::Left:
			{
				if (CharacterOwner->EnergyLeftCurrent > 0.0f)
				{
					CharacterOwner->DrainGunEnergy(EnergyDrainRate, false);
					bCanShoot = true;
				}
				else
				{
					StopFire();
				}
				break;
			}
		}
	}

	if (!bCanShoot)
	{
		return;
	}

	TargetLocation = (End);

	float NewAccuracy = Spread - Accuracy;

	FVector Offset = TargetLocation;
	Offset.X += FMath::RandRange(-NewAccuracy, NewAccuracy);
	Offset.Y += FMath::RandRange(-NewAccuracy, NewAccuracy);
	Offset.Z += FMath::RandRange(-NewAccuracy, NewAccuracy);

	AimTrace(Offset);

	SpawnShot(Offset);

	bIsFired = true;
	bIsRecoil = false;
	bIsFiring = true;
}

void AGunBase::StopFire()
{
	bIsFiring = false;
	bWantsToFire = false;

	if (IsValid(CharacterOwner))
	{
		switch (GunHand)
		{
			case EGunHand::None:
			{
				break;
			}
			case EGunHand::Right:
			{
				CharacterOwner->SetRechargeTimer(true);
				break;
			}
			case EGunHand::Left:
			{
				CharacterOwner->SetRechargeTimer(false);
				break;
			}
		}
	}
}

void AGunBase::SpawnShot(FVector End)
{
	UWorld* const World = GetWorld();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = GetInstigator();

	if (!IsValid(World))
	{
		return;
	}

	if (ShotPositions.Num() == 0)
	{
		return;
	}

	for (int Itr = 0; Itr < ShotPositions.Num(); ++Itr)
	{
		FRotator TargetRotation;
		FVector Start = ShotPositions[Itr]->GetComponentLocation();
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		ABulletBase* Shot = World->SpawnActor<ABulletBase>(BulletClass,
			ShotPositions[Itr]->GetComponentLocation(), TargetRotation, ActorSpawnParams);

		if (Shot != nullptr)
		{
			//Shot->InitSpeed(BonusVelocityMultiplier);
			Shot->InitStats(BonusVelocityMultiplier, BounceCount, Gravity, Damage);
		}
	}
}

void AGunBase::UpdateGunRotation(float Delta)
{
	if (!bIsRecoil && !bIsFired)
	{
		if (bWantsToFire)
		{
			SetFire(TargetLocation);
		}
		//if (GunMeshComponent->GetComponentRotation() != FRotator::ZeroRotator)
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(GunMeshComponent->GetComponentRotation().Pitch,
			0.0f, 0.1f))
		{
			bIsRecoil = true;
		}
		else
		{
			return;
		}
	}

	FRotator CurrentRotation = GunMeshComponent->GetRelativeRotation();
	CurrentRotation.Yaw = 0.0f;
	CurrentRotation.Roll = 0.0f;
	FRotator TargetRotation = FRotator::ZeroRotator;
	TargetRotation.Pitch = 13.0f;

	if (bIsFired)
	{
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Pitch, TargetRotation.Pitch, 1.0f))
		//if (CurrentRotation == TargetRotation)
		{
			bIsFired = false;
			bIsRecoil = true;
		}
		else
		{
			//UKismetMathLibrary::FloatSpringInterp(CurrentRotation.Pitch, 13.0f, FFloatSpringState::FFloatSpringState(), 0.5f, 10.0f, Delta);
			CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, Delta, RecoilSpeed);
		}
	}

	if (bIsRecoil)
	{
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Pitch, 0.0f, 1.0f))
		{
			bIsRecoil = false;
			if (bIsFiring)
			{
				SetFire(TargetLocation);
			}
		}
		else
		{
			CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, FRotator::ZeroRotator, Delta, RecoilRecoverySpeed);
		}
	}

	//if (!UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Pitch, GetActorRotation().Pitch))
	if (GunMeshComponent->GetRelativeRotation() != CurrentRotation)
	{
		GunMeshComponent->SetRelativeRotation(CurrentRotation);
	}
}

