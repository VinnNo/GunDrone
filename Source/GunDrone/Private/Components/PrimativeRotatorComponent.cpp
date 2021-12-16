// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PrimativeRotatorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPrimativeRotatorComponent::UPrimativeRotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPrimativeRotatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPrimativeRotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	UpdateRotatingComponents(DeltaTime);
}

void UPrimativeRotatorComponent::UpdateRotatingComponents(float Delta)
{
	for (int Itr = 0; Itr < RotatingComponents.Num(); Itr++)
	{
		if (!IsValid(RotatingComponents[Itr]))
		{
			return;
		}
		FRotator CurrentRotation = RotatingComponents[Itr]->GetRelativeRotation();
		FRotator TargetRotation = CurrentRotation + RotationSpeeds[Itr];
		CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, 
			TargetRotation, Delta, InterpolationSpeeds[Itr]);
		RotatingComponents[Itr]->SetRelativeRotation(CurrentRotation);
	}

	if (bRotateComponentToActor)
	{
		if (TargetRotationComponents.Num() > 0)
		{
			for (int Itr = 0; Itr < TargetRotationComponents.Num(); ++Itr)
			{
				FVector Velocity = GetOwner()->GetVelocity();
				FVector Start = TargetRotationComponents[Itr]->GetComponentLocation();
				FVector End;

				switch (TargetedRotationTypes[Itr])
				{
					case EEntityTargetRotation::None:
					{
						return;
						break;
					}
					case EEntityTargetRotation::Actor:
					{
						if (IsValid(TargetActor))
						{
							End = TargetActor->GetActorLocation();
						}
						break;
					}
					case EEntityTargetRotation::Forward:
					{
						if (UKismetMathLibrary::Vector_IsNearlyZero(Velocity))
						{
							if (UKismetMathLibrary::Vector_IsNearlyZero(Velocity))
							{
								Velocity = TargetRotationComponents[Itr]->GetForwardVector();
							}
							else
							{
								Velocity = LastForwardVectors[Itr];
							}
						}
						else
						{
							LastForwardVectors[Itr] = Velocity;
						}
						FRotator Dir = Velocity.Rotation();
						End = GetOwner()->GetActorLocation() + Dir.Vector();
						break;
					}
				}

				FRotator CurrentRotation = TargetRotationComponents[Itr]->GetComponentRotation();
				FRotator TargetRotation;
				TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

				switch (TargetedRotationTypes[Itr])
				{
					case EEntityTargetRotation::None:
					{
						return;
						break;
					}
					case EEntityTargetRotation::Actor:
					{
						if (bOnlyRotateComponentYaw[Itr])
						{
							TargetRotation.Pitch = 0.0f;
							TargetRotation.Roll = 0.0f;
						}
						break;
					}
					case EEntityTargetRotation::Forward:
					{
						TargetRotation.Pitch = 0.0f;
						TargetRotation.Roll = 0.0f;
						break;
					}
				}

				CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation,
					TargetRotation, Delta, TargetedInterpolationSpeeds[Itr]);

				TargetRotationComponents[Itr]->SetWorldRotation(CurrentRotation);
			}
		}
	}
}

void UPrimativeRotatorComponent::SetRotatingComponent(UPrimitiveComponent* InComponent, 
	FRotator InRotationSpeed, float InInterpolationSpeed)
{
	if (!IsValid(InComponent))
	{
		return;
	}

	if (RotatingComponents.Contains(InComponent))
	{
		int Index = -1;
		for (int Itr = 0; Itr < RotatingComponents.Num(); ++Itr)
		{
			if (InComponent == RotatingComponents[Itr])
			{
				Index = Itr;
			}
		}

		if (Index != -1)
		{

			RotationSpeeds[Index] = InRotationSpeed;
			InterpolationSpeeds[Index] = InInterpolationSpeed;
		}
		return;
	}

	RotatingComponents.Add(InComponent);
	RotationSpeeds.Add(InRotationSpeed);
	InterpolationSpeeds.Add(InInterpolationSpeed);
}

void UPrimativeRotatorComponent::RemoveRotatingComponent(UPrimitiveComponent* InComponent)
{
	if (!IsValid(InComponent))
	{
		return;
	}

	if (RotatingComponents.Contains(InComponent))
	{
		int Index = -1;
		for (int Itr = 0; Itr < RotatingComponents.Num(); ++Itr)
		{
			if (InComponent == RotatingComponents[Itr])
			{
				Index = Itr;
			}
		}

		if (Index != -1)
		{
			RotationSpeeds.Remove(RotationSpeeds[Index]);
			InterpolationSpeeds.Remove(InterpolationSpeeds[Index]);
			RotatingComponents.Remove(InComponent);
		}
	}
}

void UPrimativeRotatorComponent::SetTargetedRotation(UPrimitiveComponent* InComponent, 
	AActor* InActor, float InInterpolationSpeed, 
	EEntityTargetRotation::Type InType, bool bOnlyUpdateYaw)
{
	if (!IsValid(InComponent))
	{
		return;
	}

	if (TargetRotationComponents.Contains(InComponent))
	{
		int Index = -1;
		for (int Itr = 0; Itr < TargetRotationComponents.Num(); ++Itr)
		{
			if (InComponent == TargetRotationComponents[Itr])
			{
				Index = Itr;
			}
		}

		if (Index != -1)
		{
			TargetedInterpolationSpeeds[Index] = InInterpolationSpeed;
			TargetedRotationTypes[Index] = InType;
			bOnlyRotateComponentYaw[Index] = bOnlyUpdateYaw;
		}
	}

	TargetRotationComponents.Add(InComponent);
	if (IsValid(InActor))
	{
		TargetActor = InActor;
	}
	TargetedInterpolationSpeeds.Add(InInterpolationSpeed);
	TargetedRotationTypes.Add(InType);
	LastForwardVectors.Add(FVector::ZeroVector);
	bOnlyRotateComponentYaw.Add(bOnlyUpdateYaw);
}

