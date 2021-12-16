// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EntityFlyer.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Character/CharacterBase.h"
#include "DrawDebugHelpers.h" 

// Sets default values
AEntityFlyer::AEntityFlyer()
{
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	DebugArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DebugArrow"));
	DebugArrow->SetupAttachment(RootComponent);

	MovementComponenent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEntityFlyer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntityFlyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateEntityStates(DeltaTime);


}

void AEntityFlyer::UpdateEntityStates(float Delta)
{

	switch (EntityAIState)
	{
		case EEntityStates::Idle:
		{



			break;
		}
		case EEntityStates::Patrol:
		{
			FVector TargetSpeed = (GetActorForwardVector() * SpeedPatrol);
			MovementComponenent->Velocity = TargetSpeed;
			if (TraceForPlayer())
			{
				SetEntityState(EEntityStates::Chase);
			}
			break;
		}
		case EEntityStates::Chase:
		{
			if (TraceForPlayer())
			{
				if (DistanceToPlayer > ChaseDistance)
				{
					CurrentRotation = GetActorRotation();
					SetTargetRotation(TargetCharacter->GetActorLocation());
					CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, 
						TargetRotation, Delta, 10.0f);
					SetActorRotation(CurrentRotation);
					FVector Direction = GetActorForwardVector();
					if (DistanceToPlayer > (ChaseDistance * 3.0f))
					{
						Direction *= (SpeedChase * 3.0f);
					}
					else
					{
						Direction *= SpeedChase;
					}

					MovementComponenent->Velocity = Direction;
				}
				else
				{
					SetEntityState(EEntityStates::Engage);
				}
			}
			else
			{
				SetEntityState(EEntityStates::Patrol);
			}
			break;
		}
		case EEntityStates::Engage:
		{
			if (TraceForPlayer())
			{
				if (DistanceToPlayer > (ChaseDistance * 1.5f))
				{
					SetEntityState(EEntityStates::Chase);
				}

				if (EngagementTime < EngagementTimeMax)
				{
					EngagementTime += Delta;
				}
				else
				{
					SetEntityState(EEntityStates::Engage);
				}

				if (bUpdateTargetToPlayer)
				{
					TargetLocation = TargetCharacter->GetActorLocation();
				}

				switch (EngagementType)
				{
					case EEntityEngagement::Approach:
					{
						CurrentRotation = GetActorRotation();
						SetTargetRotation(TargetLocation);
						SetActorRotation(UpdateRotation(Delta));
						FVector Direction = GetActorForwardVector();
						MovementComponenent->Velocity = (Direction * SpeedEngage);
						break;
					}
					case EEntityEngagement::Strafe:
					{
						SetTargetRotation(TargetLocation);
						CurrentRotation = GetActorRotation();
						SetActorRotation(UpdateRotation(Delta));
						//SetActorRotation(DefinedRotation);

						FVector MovementDirection = (EngagementDirection);
						FRotator MovementRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
							GetActorLocation() + (MovementDirection * SpeedEngage));

						FVector Direction = (MovementRotation.Vector());
						MovementComponenent->Velocity = (Direction * SpeedEngage);

						break;
					}
					case EEntityEngagement::Flee:
					{
						CurrentRotation = GetActorRotation();
						SetTargetRotation(TargetLocation);
						SetActorRotation(UpdateRotation(Delta));
						FVector Direction = GetActorForwardVector() * -1.0f;
						MovementComponenent->Velocity = (Direction * SpeedEngage);
						break;
					}
				}

			}
			else
			{
				SetEntityState(EEntityStates::Patrol);
			}
			break;
		}
	}



}

void AEntityFlyer::SetEntityState(TEnumAsByte<EEntityStates::Type> InState)
{
	switch (InState)
	{
		case EEntityStates::Idle:
		{
			MovementComponenent->bIsHomingProjectile = true;
			MovementComponenent->bRotationFollowsVelocity = true;

			break;
		}
		case EEntityStates::Patrol:
		{
			//SetTargetRotation((GetActorForwardVector() * SpeedPatrol));
			MovementComponenent->bIsHomingProjectile = true;
			MovementComponenent->bRotationFollowsVelocity = true;
			break;
		}
		case EEntityStates::Chase:
		{
			MovementComponenent->bIsHomingProjectile = false;
			MovementComponenent->bRotationFollowsVelocity = false;

			break;
		}
		case EEntityStates::Engage:
		{
			MovementComponenent->bIsHomingProjectile = false;
			MovementComponenent->bRotationFollowsVelocity = false;
			EngagementTime = 0.0f;
			EngagementTimeMax = FMath::RandRange(EngagementRange.X, EngagementRange.Y);

			switch (EngagementType)
			{
				case EEntityEngagement::Approach:
				{
					if (TargetCharacter != nullptr)
					{
						
						FRotator RotationDirection = UKismetMathLibrary::FindLookAtRotation(
							GetActorLocation(), TargetCharacter->GetActorLocation()
						);
						EngagementDirection = RotationDirection.Vector();
					}
					break;
				}
				case EEntityEngagement::Strafe:
				{
					EngagementDirection = RandomizeEngagementDirection();
					break;
				}
				case EEntityEngagement::Flee:
				{
					FRotator RotationDirection = UKismetMathLibrary::FindLookAtRotation(
						TargetCharacter->GetActorLocation(), GetActorLocation()
					);
					EngagementDirection = RotationDirection.Vector();
					break;
				}
			}


			if (FMath::RandRange(1.f, 100.f) > 50.f)
			{
				StrafeDirection = 1;
			}
			else
			{
				StrafeDirection = -1;
			}

			break;
		}
	}
	if (EntityAIState != InState)
	{
		OnEntityStateChanged(InState);
	}
	EntityAIState = InState;
}

void AEntityFlyer::SetEntityEngagement(TEnumAsByte<EEntityEngagement::Type> InEngagement)
{
	if (EngagementType != InEngagement)
	{
		EngagementType = InEngagement;
		OnEntityEngagementChanged(EngagementType);
	}
}

FVector AEntityFlyer::RandomizeEngagementDirection()
{
	float RNG = FMath::RandRange(1.0f, 100.0f);

	float RNGX = FMath::RandRange(-1000.0f, 1000.0f);
	float RNGY = FMath::RandRange(-1000.0f, 1000.0f);
	float RNGZ = FMath::RandRange(-1000.0f, 1000.0f);

	FVector RandomDirection = FVector(RNGX, RNGY, RNGZ);
	return RandomDirection;

	if (RNG > 80)
	{
		return GetActorForwardVector();
	}
	else if (RNG > 60)
	{
		return GetActorRightVector();
	}
	else if (RNG > 40)
	{
		return GetActorRightVector() * -1.0f;
	}
	else if (RNG > 20)
	{
		return GetActorUpVector();
	}
	else
	{
		return GetActorUpVector() * -1.0f;
	}
}

void AEntityFlyer::SetTargetRotation(FVector InTargetLocation)
{
	FVector Start = GetActorLocation();
	FVector End = InTargetLocation;

	TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

}

FRotator AEntityFlyer::UpdateRotation(float Delta)
{
	FRotator Start = CurrentRotation;
	FRotator End = TargetRotation;

	Start = UKismetMathLibrary::RInterpTo(Start, End, Delta, RotationSpeed);

	DefinedRotation = Start;

	return DefinedRotation;
}

void AEntityFlyer::UpdateActionTime(float Delta)
{
	if (!bUseActionTimer)
	{
		return;
	}
	if (ActionTime < ActionTimeMax)
	{
		ActionTime += Delta;
	}
	else
	{
		OnActionTime();
		bUseActionTimer = false;
		ActionTime = 0.0f;
	}
}

bool AEntityFlyer::TraceForPlayer(bool bDrawDebug)
{
	if (!IsValid(TargetCharacter))
	{
		return false;
	}

	float Distance = 10000.0f; // Just Some big number :/
	float LineThickness = 6.0f;
	uint8 Depth = 0;
	float LifeTime = 5.0f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FHitResult OutHit;

	FVector Start = GetActorLocation();
	FVector End = TargetCharacter->GetActorLocation();

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, 
		End, ECC_Visibility, CollisionParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, LifeTime, Depth, LineThickness);
	}

	AActor* HitActor = OutHit.GetActor();

	if (IsValid(HitActor))
	{
		ACharacterBase* HitCharacter = Cast<ACharacterBase>(HitActor);
		if (IsValid(HitCharacter))
		{
			DistanceToPlayer = OutHit.Distance;
			return true;
		}
		return false;
	}
	return false;
}

