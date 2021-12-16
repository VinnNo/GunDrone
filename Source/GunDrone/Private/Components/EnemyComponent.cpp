// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EnemyComponent.h"

#include "../../Public/Bullets/BulletBase.h"

#include "Engine.h"

// Sets default values for this component's properties
UEnemyComponent::UEnemyComponent()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitBindings();

	HealthCurrent = HealthMax;
	
}


// Called every frame
void UEnemyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	HitFlashUpdate(DeltaTime);
}

void UEnemyComponent::InitBindings()
{
	if (GetOwner() != nullptr)
	{
		UPrimitiveComponent* PC = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
		if (PC != nullptr)
		{
			CollisionComponent = PC;
		}
	}

	if (CollisionComponent != nullptr)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UEnemyComponent::OnOverlapBinding);
		CollisionComponent->OnComponentHit.AddDynamic(this, &UEnemyComponent::OnHitBinding);
	}
}

void UEnemyComponent::OnHitBinding(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void UEnemyComponent::OnOverlapBinding(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, OverlappedComponent->GetName());

	ABulletBase* Shot = Cast<ABulletBase>(OtherActor);
	if (Shot != nullptr)
	{
		if (Shot->BulletType != EBulletType::Player)
		{
			return;
		}
		OnEnemyHit.Broadcast();
		TakeDamage(Shot->Damage);
		Shot->BulletDeath();
	}
}

void UEnemyComponent::TakeDamage(float InDamage)
{
	HealthCurrent -= InDamage;
	if (HealthCurrent < 0.0f)
	{
		OnEnemyDeath.Broadcast();
	}
	else
	{
		OnEnemyHit.Broadcast();
	}
}

void UEnemyComponent::HitFlashUpdate(float Delta)
{
	if (!bIsHitflash)
	{
		return;
	}

	float Alpha = Delta * HitFlashSpeed;

	if (IsValid(HitFlashMaterial))
	{
		HitFlashStart.X = FMath::Lerp(HitFlashStart.X, HitFlashEnd.X, Alpha);
		HitFlashStart.Y = FMath::Lerp(HitFlashStart.Y, HitFlashEnd.Y, Alpha);
		HitFlashStart.Z = FMath::Lerp(HitFlashStart.Z, HitFlashEnd.Z, Alpha);

		FLinearColor NewColor = FLinearColor(HitFlashStart.X, HitFlashStart.Y, HitFlashStart.Z, 1.0f);

		HitFlashMaterial->SetVectorParameterValue("Color", NewColor);

	}
}

void UEnemyComponent::SetHitFlash(bool bSetActive, 
	UPrimitiveComponent* InComp, FVector Start, FVector End)
{
	if (!bSetActive)
	{
		bIsHitflash = false;
		if (HitFlashMaterial != nullptr)
		{
			FLinearColor NewColor = FLinearColor(End.X, End.Y, End.Z, 1.0f);
			HitFlashMaterial->SetVectorParameterValue("Color", NewColor);
			HitFlashMaterial = nullptr;
		}
		return;
	}

	UMaterialInterface* Mat = InComp->GetMaterial(0);
	if (IsValid(Mat))
	{
		HitFlashMaterial = InComp->CreateDynamicMaterialInstance(0, Mat);
		HitFlashStart = Start;
		HitFlashEnd = End;
		bIsHitflash = true;
	}
}
