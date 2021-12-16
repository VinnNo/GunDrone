// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyBase.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Engine.h"



// Sets default values
AEnemyBase::AEnemyBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = SceneRoot;


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionShapeBox"));
	BoxCollision->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionShapeSphere"));
	SphereCollision->SetupAttachment(RootComponent);


	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionShapeCapsule"));
	CapsuleCollision->SetupAttachment(RootComponent);


	//InitShapes();

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



void AEnemyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AEnemyBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, OverlappedComponent->GetName());
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitShapes();
	BindCollisions();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::InitShapes()
{

	switch (CollisionType)
	{
		case EBaseCollisionShape::Box:
		{
			if (SphereCollision != nullptr)
			{
				SphereCollision->DestroyComponent(true);
			}
			if (CapsuleCollision != nullptr)
			{
				CapsuleCollision->DestroyComponent(true);
			}
			break;
		}
		case EBaseCollisionShape::Sphere:
		{
			if (BoxCollision != nullptr)
			{
				BoxCollision->DestroyComponent(true);
			}
			if (CapsuleCollision != nullptr)
			{
				CapsuleCollision->DestroyComponent(true);
			}
			break;
		}
		case EBaseCollisionShape::Capsule:
		{
			if (BoxCollision != nullptr)
			{
				BoxCollision->DestroyComponent(true);
			}
			if (SphereCollision != nullptr)
			{
				SphereCollision->DestroyComponent(true);
			}
			break;
		}
	}
}

void AEnemyBase::BindCollisions()
{

	switch (CollisionType)
	{
		case EBaseCollisionShape::Box:
		{
			if (IsValid(BoxCollision))
			{
				BoxCollision->OnComponentHit.AddDynamic(this, &AEnemyBase::OnHit);
				BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlap);
			}
			break;
		}
		case EBaseCollisionShape::Sphere:
		{
			if (IsValid(SphereCollision))
			{
				SphereCollision->OnComponentHit.AddDynamic(this, &AEnemyBase::OnHit);
				SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlap);
			}
			break;
		}
		case EBaseCollisionShape::Capsule:
		{
			if (IsValid(CapsuleCollision))
			{
				CapsuleCollision->OnComponentHit.AddDynamic(this, &AEnemyBase::OnHit);
				CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlap);
			}
			break;
		}
	}
}

