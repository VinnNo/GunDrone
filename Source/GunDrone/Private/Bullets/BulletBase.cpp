// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullets/BulletBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABulletBase::ABulletBase()
{
	RootScene = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = RootScene;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	FTimerHandle LifeHandler;
	GetWorldTimerManager().SetTimer(LifeHandler, [this]()
	{

		OnLifeTimeUp();
		Destroy();

	}, LifeTime, false);
	*/

	RootScene->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlap);
	RootScene->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);
	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ABulletBase::OnBounce);

	if (BounceCountMax != 0)
	{
		ProjectileMovement->bShouldBounce = true;
	}
	else
	{
		ProjectileMovement->bShouldBounce = false;
	}
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimer(DeltaTime);
}

void ABulletBase::InitSpeed(float Value)
{
	GetMovement()->MaxSpeed += GetMovement()->MaxSpeed * Value;
	GetMovement()->InitialSpeed = GetMovement()->MaxSpeed;
	GetMovement()->Velocity = GetActorForwardVector() * GetMovement()->MaxSpeed;
}

void ABulletBase::InitStats(float InSpeedMultiplier, int InBounce, 
	float InGravity, float InDamage)
{
	// Speed
	GetMovement()->MaxSpeed += GetMovement()->MaxSpeed * InSpeedMultiplier;
	GetMovement()->InitialSpeed = GetMovement()->MaxSpeed;
	GetMovement()->Velocity = GetActorForwardVector() * GetMovement()->MaxSpeed;

	// Bounces
	BounceCountMax = InBounce;
	if (BounceCountMax != 0)
	{
		ProjectileMovement->bShouldBounce = true;
	}
	else
	{
		ProjectileMovement->bShouldBounce = false;
	}

	// Gravity
	GetMovement()->ProjectileGravityScale = InGravity;

	// Damage
	Damage = InDamage;

}

void ABulletBase::UpdateTimer(float Delta)
{
	if (!bUseLifeTime)
	{
		return;
	}
	if (LifeTimeCurrent < LifeTime)
	{
		LifeTimeCurrent += 1.0f * Delta;
	}
	else
	{
		OnLifeTimeUp();
		Destroy();
	}
}

void ABulletBase::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	BounceCountCurrent += 1;
}

void ABulletBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bWaitOnDestroy)
	{
		return;
	}
	if (BounceCountCurrent == BounceCountMax)
	{
		BulletDeath();
	}
}

void ABulletBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABulletBase::BulletDeath()
{
	if (bWaitOnDestroy)
	{
		return;
	}
	OnDeath();
	Destroy();
}

