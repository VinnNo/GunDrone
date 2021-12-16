// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EntityCharacter.h"
#include "AIController.h"
#include "Character/CharacterBase.h"
#include "DrawDebugHelpers.h" 

// Sets default values
AEntityCharacter::AEntityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEntityCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateEntityStates(DeltaTime);
	AAIController* AI = Cast<AAIController>(GetController());
	if (IsValid(AI))
	{
		//AI->OnMoveCompleted.AddDynamic(this, &AEntityCharacter::OnMoveCompleted);
	}

}

// Called to bind functionality to input
void AEntityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEntityCharacter::UpdateEntityStates(float Delta)
{
	switch (EntityAIState)
	{
		case EEntityStates::Idle:
		{

			break;
		}
		case EEntityStates::Patrol:
		{

			break;
		}
		case EEntityStates::Chase:
		{
			if (bUpdateTargetToPlayer)
			{
				if (IsValid(TargetCharacter))
				{
					TargetLocation = TargetCharacter->GetActorLocation();
				}
			}

			AAIController* AI = Cast<AAIController>(GetController());
			if (AI != nullptr)
			{
				AI->MoveToLocation(TargetLocation, ChaseDistance - 50.0f);
			}

			float Distance = FVector::Distance(GetActorLocation(), TargetLocation);

			if (Distance <= ChaseDistance)
			{
				OnMovementCompleted(EntityAIState);
			}

			AActor* TraceTargetPrev = TraceTarget;
			TraceForPlayer();

			if (TraceTarget != TraceTargetPrev)
			{
				ACharacterBase* Character = Cast<ACharacterBase>(TraceTarget);
				if (IsValid(Character))
				{
					OnPlayerTargetAcquired();
				}
			}

			break;
		}
		case EEntityStates::Engage:
		{
			if (bUpdateTargetToPlayer)
			{
				if (IsValid(TargetCharacter))
				{
					TargetLocation = TargetCharacter->GetActorLocation();
				}
			}

			float Distance = FVector::Distance(GetActorLocation(), TargetLocation);

			if (Distance > ChaseDistance)
			{
				SetEntityState(EEntityStates::Chase);
			}

			AActor* TraceTargetPrev = TraceTarget;

			if (TraceForPlayer())
			{
				if (TraceTarget != TraceTargetPrev)
				{
					ACharacterBase* Character = Cast<ACharacterBase>(TraceTarget);
					if (IsValid(Character))
					{
						OnPlayerTargetAcquired();
					}
				}
			}

			break;
		}
	}

}

void AEntityCharacter::SetEntityState(TEnumAsByte<EEntityStates::Type> InState)
{
	switch (InState)
	{
		case EEntityStates::Idle:
		{

			break;
		}
		case EEntityStates::Patrol:
		{

			break;
		}
		case EEntityStates::Chase:
		{

			break;
		}
		case EEntityStates::Engage:
		{

			break;
		}
	}

	if (EntityAIState != InState)
	{
		OnEntityStateChanged(InState);
	}
	EntityAIState = InState;
}

bool AEntityCharacter::TraceForPlayer(bool bDrawDebug)
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

	FVector Start;
	
	if (bTraceFromComponent)
	{
		if (IsValid(TraceComponent))
		{
			Start = TraceComponent->GetComponentLocation();
		}
	}
	else
	{
		Start = GetActorLocation();
	}

	FVector End;

	if (bTraceFromComponent)
	{
		if (IsValid(TraceComponent))
		{
			End = TraceComponent->GetForwardVector() * SightDistance;
		}
	}
	else
	{
		End = TargetCharacter->GetActorLocation();
	}

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start,
		End, ECC_Visibility, CollisionParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, LifeTime, Depth, LineThickness);
	}

	AActor* HitActor = OutHit.GetActor();
	TraceTarget = HitActor;

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