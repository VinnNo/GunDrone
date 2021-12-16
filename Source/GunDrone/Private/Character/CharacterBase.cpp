// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"

#include "GameFramework/InputSettings.h"

#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "../../Public/Guns/GunBase.h"
#include "../../Public/Bullets/BulletBase.h"
#include "../../Public/Data/SaveGameBase.h"

#include "DrawDebugHelpers.h" 

// Sets default values
ACharacterBase::ACharacterBase()
{
	HeadMount = CreateDefaultSubobject<USceneComponent>(TEXT("HeadMount"));
	HeadMount->SetupAttachment(RootComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(HeadMount);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshObject(TEXT("/Game/Geometry/SM_DroneBody.SM_DroneBody"));

	if (BodyMeshObject.Succeeded())
	{
		BodyMesh->SetStaticMesh(BodyMeshObject.Object);
	}

	RightArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("RightArm"));
	RightArm->bEnableCameraLag = true;
	RightArm->CameraLagSpeed = 50.0f;
	RightArm->CameraLagMaxDistance = 2.5f;
	RightArm->TargetArmLength = -75.0f;


	LeftArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LeftArm"));
	LeftArm->bEnableCameraLag = true;
	LeftArm->CameraLagSpeed = 50.0f;
	LeftArm->CameraLagMaxDistance = 2.5f;
	LeftArm->TargetArmLength = -75.0f;

	if (BodyMeshObject.Succeeded())
	{
		RightArm->SetupAttachment(BodyMesh, TEXT("RightMount"));
		LeftArm->SetupAttachment(BodyMesh, TEXT("LeftMount"));
	}
	else
	{
		RightArm->SetupAttachment(BodyMesh);
		LeftArm->SetupAttachment(BodyMesh);
	}
	
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(HeadMount);
	FPSCamera->bUsePawnControlRotation = false;
	FPSCamera->SetRelativeLocation(FVector(40.0f, 0.0f, 20.0f));

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	//StaticMesh'/Game/Geometry/SM_DroneBody.SM_DroneBody'

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	JumpCountCurrent = JumpCountMax + JumpCountBoosted;
	HealthCurrent = HealthMax + HealthBoosted;
	EnergyRightCurrent = (EnergyMax + EnergyBoosted);
	EnergyLeftCurrent = (EnergyMax + EnergyBoosted);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnOverlapBinding);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTrace();
	//UpdateBobbing(DeltaTime);
	UpdateCameraRotation(DeltaTime);

	UpdateKineticEnergy();

	RechargeRightGunEnergy(DeltaTime);
	RechargeLeftGunEnergy(DeltaTime);
}

void ACharacterBase::UpdateCameraRotation(float Delta)
{
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = GetController()->GetControlRotation();
	
	if (CurrentRotation.Yaw != TargetRotation.Yaw)
	{
		FRotator temp = FRotator::ZeroRotator;
		temp.Yaw = TargetRotation.Yaw;
		SetActorRotation(temp);
	}
	CurrentRotation = HeadMount->GetRelativeRotation();
	CurrentRotation.Yaw = 0.0f;
	TargetRotation.Yaw = 0.0f;
	if (CurrentRotation != TargetRotation)
	{
		HeadMount->SetRelativeRotation(TargetRotation);
		//FPSCamera->SetWorldRotation(TargetRotation);
	}
}

void ACharacterBase::RechargeRightGunEnergy(float Delta)
{
	if (!bGunRightRecharging)
	{

		return;
	}

	if (EnergyRightCurrent < GetMaxEnergy())
	{
		EnergyRightCurrent += GunEnergyRechargeRate * Delta;
		if (EnergyRightCurrent > GetMaxEnergy())
		{
			EnergyRightCurrent = GetMaxEnergy();
			bGunRightRecharging = false;
		}
		OnRightEnergyChanged(GetRightEnergyAsPercent());
	}
}

void ACharacterBase::RechargeLeftGunEnergy(float Delta)
{
	if (!bGunLeftRecharging)
	{
		return;
	}

	if (EnergyLeftCurrent < GetMaxEnergy())
	{
		EnergyLeftCurrent += GunEnergyRechargeRate * Delta;
		if (EnergyLeftCurrent > GetMaxEnergy())
		{
			EnergyLeftCurrent = GetMaxEnergy();
			bGunLeftRecharging = false;
		}
		OnLeftEnergyChanged(GetLeftEnergyAsPercent());
	}
}

void ACharacterBase::SetRechargeTimer(bool bIsRight)
{
	if (bIsRight)
	{
		if (GetWorldTimerManager().IsTimerActive(RightGunDelayHandle))
		{
			GetWorldTimerManager().ClearTimer(RightGunDelayHandle);
		}

		GetWorldTimerManager().SetTimer(RightGunDelayHandle, [this]()
		{
			bGunRightRecharging = true;
		}, EnergyRechargeDelay, false);
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(LeftGunDelayHandle))
		{
			GetWorldTimerManager().ClearTimer(LeftGunDelayHandle);
		}

		GetWorldTimerManager().SetTimer(LeftGunDelayHandle, [this]()
		{
			bGunLeftRecharging = true;
		}, EnergyRechargeDelay, false);
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::InputMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::InputMoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::InputJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::InputJumpReleased);

	PlayerInputComponent->BindAction("FireLeft", IE_Pressed, this, &ACharacterBase::InputFireLeftPressed);
	PlayerInputComponent->BindAction("FireLeft", IE_Released, this, &ACharacterBase::InputFireLeftReleased);

	PlayerInputComponent->BindAction("FireRight", IE_Pressed, this, &ACharacterBase::InputFireRightPressed);
	PlayerInputComponent->BindAction("FireRight", IE_Released, this, &ACharacterBase::InputFireRightReleased);
}

void ACharacterBase::InputMoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACharacterBase::InputMoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACharacterBase::UpdateKineticEnergy()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (GetCharacterMovement()->Velocity.X == 0.0f
		&& GetCharacterMovement()->Velocity.Y == 0.0f)
	{
		return;
	}

	float Time = World->GetDeltaSeconds();

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		if (EnergyRightCurrent < GetMaxEnergy())
		{
			EnergyRightCurrent += KineticEnergyRechargeRate * Time;
			if (EnergyRightCurrent > GetMaxEnergy())
			{
				EnergyRightCurrent = GetMaxEnergy();
			}
			OnRightEnergyChanged(GetRightEnergyAsPercent());
		}

		if (EnergyLeftCurrent < GetMaxEnergy())
		{
			EnergyLeftCurrent += KineticEnergyRechargeRate * Time;
			if (EnergyLeftCurrent > GetMaxEnergy())
			{
				EnergyLeftCurrent = GetMaxEnergy();
			}
			OnLeftEnergyChanged(GetLeftEnergyAsPercent());
		}
	}
}

void ACharacterBase::InputJumpPressed()
{
	FVector LaunchVelocity = GetCharacterMovement()->Velocity;
	LaunchVelocity.Z = GetCharacterMovement()->JumpZVelocity;
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		LaunchCharacter(LaunchVelocity, true, true);
		////OnLaunched();
		//DoLaunch(LaunchVelocity);
		JumpCountCurrent -= 1;
		OnJumpCountCurrentChanged(GetRemainingJumps());
		return;
	}
	else
	{
		if (JumpCountCurrent > 0)
		{
			JumpCountCurrent -= 1;
			LaunchCharacter(LaunchVelocity, true, true);
			//OnLaunched();
			//DoLaunch(LaunchVelocity);
			OnJumpCountCurrentChanged(GetRemainingJumps());
		}
	}
}

void ACharacterBase::InputJumpReleased()
{
	if (GetCharacterMovement()->Velocity.Z > 0.0f)
	{
		GetCharacterMovement()->Velocity.Z *= 0.25f;
	}
}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::OnLanded(Hit);
	JumpCountCurrent = JumpCountMax + JumpCountBoosted;
	OnJumpCountCurrentChanged(GetRemainingJumps());
}

void ACharacterBase::InputFireRightPressed()
{
	if (IsValid(GunRight))
	{
		GunRight->SetFire(HitLocation);
		bGunRightRecharging = false;
		if (GetWorldTimerManager().IsTimerActive(RightGunDelayHandle))
		{
			GetWorldTimerManager().ClearTimer(RightGunDelayHandle);
		}
	}
}

void ACharacterBase::InputFireRightReleased()
{
	if (IsValid(GunRight))
	{
		GunRight->StopFire();
		GunRight->bWantsToFire = false;
	}
}

void ACharacterBase::InputFireLeftPressed()
{
	if (IsValid(GunLeft))
	{
		GunLeft->SetFire(HitLocation);
		bGunLeftRecharging = false;
		if (GetWorldTimerManager().IsTimerActive(LeftGunDelayHandle))
		{
			GetWorldTimerManager().ClearTimer(LeftGunDelayHandle);
		}
	}
}

void ACharacterBase::InputFireLeftReleased()
{
	if (IsValid(GunLeft))
	{
		GunLeft->StopFire();
	}
}

void ACharacterBase::AimTrace()
{
	float Distance = 10000.0f; // Just Some big number :/
	float LineThickness = 6.0f;
	uint8 Depth = 0;
	float LifeTime = 5.0f;

	const APlayerController* const PlayerController = Cast<const APlayerController>(GetController());
	FVector WorldLocation, WorldDirection;
	int ScreenX, ScreenY;
	PlayerController->GetViewportSize(ScreenX, ScreenY);

	// Deproject
	PlayerController->DeprojectScreenPositionToWorld(ScreenX * 0.5f, ScreenY * 0.5f, WorldLocation, WorldDirection);

	FVector Start = WorldLocation + (WorldDirection);
	FVector End = Start + (WorldDirection * Distance);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FHitResult OutHit;

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	bool bUpdateTarget = false;

	if (bIsHit)
	{
		if (OutHit.bBlockingHit)
		{
			if (OutHit.GetActor() != nullptr)
			{
				//TargetActor = OutHit.GetActor();
			}
			else
			{
				//TargetActor = nullptr;
			}
			if (bDrawAimTrace && bIsDebug)
			{
				End = OutHit.Location;
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, LifeTime, Depth, LineThickness);
				FVector Start2 = OutHit.Location;
				float LineLengthRemaining = OutHit.Distance;
				FVector End2 = Start2 + (WorldDirection * (Distance - LineLengthRemaining));
				DrawDebugLine(GetWorld(), Start2, End2, FColor::Red, false, LifeTime, Depth, LineThickness);
			}
			End = OutHit.Location;
		}
	}
	else
	{
		End = Start + (WorldDirection * Distance);
		if (bDrawAimTrace && bIsDebug)
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, LifeTime, Depth, LineThickness);
	}

	HitLocation = End;

	//TargetActor = OutHit.GetActor();
	//TargetDistanceRange = OutHit.Distance;
}

void ACharacterBase::EquipGun(bool bIsRight)
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


	if (bIsRight && GunRightClass != nullptr)
	{

		AActor* Gun = World->SpawnActor<AGunBase>(GunRightClass, 
			RightArm->GetComponentLocation(), RightArm->GetComponentRotation(), ActorSpawnParams);

		if (Gun != nullptr)
		{
			if (GunRight != nullptr)
			{
				GunRight->Destroy();
			}
			GunRight = Cast<AGunBase>(Gun);
			//GunRight->bDrawAimTrace = true;
			GunRight->CharacterOwner = this;
			GunRight->AttachToComponent(RightArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
			GunRight->GunHand = EGunHand::Right;
		}

	}

	if (!bIsRight && GunLeftClass != nullptr)
	{

		AActor* Gun = World->SpawnActor<AGunBase>(GunLeftClass,
			LeftArm->GetComponentLocation(), LeftArm->GetComponentRotation(), ActorSpawnParams);

		if (Gun != nullptr)
		{
			if (GunLeft != nullptr)
			{
				GunLeft->Destroy();
			}
			GunLeft = Cast<AGunBase>(Gun);
			//GunRight->bDrawAimTrace = true;
			GunLeft->CharacterOwner = this;
			GunLeft->AttachToComponent(LeftArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
			GunLeft->GunHand = EGunHand::Left;
		}
	}

}

void ACharacterBase::SpawnGun(TSubclassOf<class AGunBase> InGun, FVector InLocation, FRotator InRotation)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = GetInstigator();

	UWorld* const World = GetWorld();

	
}

void ACharacterBase::SetJumpCounts(float Value)
{
	int JumpOffset = (JumpCountMax + JumpCountBoosted) - JumpCountCurrent;
	JumpCountBoosted += Value;
	JumpCountCurrent = (JumpCountMax + JumpCountBoosted) + JumpOffset;
	OnJumpCountMaxChanged((JumpCountCurrent + JumpCountMax));
}

void ACharacterBase::DoLaunch(FVector LaunchVelocity)
{
	FVector NewVelocity;

	if (LaunchVelocity.X == 0.0f)
	{
		NewVelocity.X = GetCharacterMovement()->Velocity.X;
	}
	if (LaunchVelocity.Y == 0.0f)
	{
		NewVelocity.Y = GetCharacterMovement()->Velocity.Y;
	}
	if (NewVelocity.Z == 0.0f)
	{
		NewVelocity.Z = GetCharacterMovement()->Velocity.Z;
	}
	LaunchCharacter(NewVelocity, true, true);
}

void ACharacterBase::OnOverlapBinding(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABulletBase* Shot = Cast<ABulletBase>(OtherActor);
	if (Shot != nullptr)
	{
		if (Shot->BulletType != EBulletType::Enemy)
		{
			return;
		}
		DamagePlayer(Shot->Damage, false, FVector::ZeroVector);
		Shot->BulletDeath();
	}
}

void ACharacterBase::HealPlayer(float Value)
{
	HealthCurrent += Value;
	if (HealthCurrent > (HealthMax + HealthBoosted))
	{
		HealthCurrent = (HealthMax + HealthBoosted);
	}
	OnHealthChanged(false, GetHealthAsPercent());
}

void ACharacterBase::DamagePlayer(float Value, bool bApplyForce, FVector Force)
{
	HealthCurrent -= Value;
	if (HealthCurrent < 0.0f)
	{
		OnPlayerDeath();
		return;
	}
	else
	{
		//OnPlayerDamaged();
		OnHealthChanged(true, GetHealthAsPercent());
	}

	if (bApplyForce)
	{
		LaunchCharacter(Force, true, true);
	}
}

void ACharacterBase::UpgradeHealth(float Value)
{
	HealthBoosted += Value;
	HealPlayer(Value * 1.5);
}

void ACharacterBase::UpgradeEnergy(float Value)
{
	EnergyBoosted += Value;
	EnergyLeftCurrent = GetMaxEnergy();
	EnergyRightCurrent = GetMaxEnergy();

	OnRightEnergyChanged(GetRightEnergyAsPercent());
	OnLeftEnergyChanged(GetLeftEnergyAsPercent());
}

void ACharacterBase::UpgradeJumps(int Value)
{
	int JumpOffset = (JumpCountMax + JumpCountBoosted) - JumpCountCurrent;
	JumpCountBoosted += Value;
	JumpCountCurrent = (JumpCountMax + JumpCountBoosted) + JumpOffset;
	OnJumpCountMaxChanged((JumpCountCurrent + JumpCountMax));
}

void ACharacterBase::HealEnergy(float Value)
{
	EnergyLeftCurrent += Value;
	if (EnergyLeftCurrent > GetMaxEnergy())
	{
		EnergyLeftCurrent = GetMaxEnergy();
	}

	EnergyRightCurrent += Value;
	if (EnergyRightCurrent > GetMaxEnergy())
	{
		EnergyRightCurrent = GetMaxEnergy();
	}

	OnRightEnergyChanged(GetRightEnergyAsPercent());
	OnLeftEnergyChanged(GetLeftEnergyAsPercent());
}

void ACharacterBase::AddPlayerData(int Value)
{
	DataCurrent += Value;
	if (DataCurrent >= DataMax)
	{
		DataPoints += 1;
		DataMax = round(DataMax + (DataMax * DataScalar));

	}
}

void ACharacterBase::DrainGunEnergy(float DrainCost, bool bIsRight)
{
	if (bIsRight)
	{
		EnergyRightCurrent -= DrainCost;
		if (EnergyRightCurrent < 0.0f)
		{
			EnergyRightCurrent = 0.0f;
		}
		OnRightEnergyChanged(GetRightEnergyAsPercent());
	}
	else
	{
		EnergyLeftCurrent -= DrainCost;
		if (EnergyLeftCurrent < 0.0f)
		{
			EnergyLeftCurrent = 0.0f;
		}
		OnLeftEnergyChanged(GetLeftEnergyAsPercent());
	}
}

float ACharacterBase::GetHealthAsPercent()
{
	const float Percent = HealthCurrent / (HealthMax + HealthBoosted);
	return Percent;
}

float ACharacterBase::GetMaxHealth()
{
	const float MaxHealth = (HealthBoosted + HealthMax);
	return MaxHealth;
}

int ACharacterBase::GetRemainingJumps()
{
	const int RemainingJumps = (JumpCountBoosted + JumpCountMax) - JumpCountCurrent;
	return RemainingJumps;
}

int ACharacterBase::GetMaxJumps()
{
	const int MaxJumps = (JumpCountBoosted + JumpCountMax);
	return MaxJumps;
}

float ACharacterBase::GetRightEnergyAsPercent()
{
	const float Percent = EnergyRightCurrent / (EnergyMax + EnergyBoosted);
	return Percent;
}

float ACharacterBase::GetLeftEnergyAsPercent()
{
	const float Percent = EnergyLeftCurrent / (EnergyMax + EnergyBoosted);
	return Percent;
}

float ACharacterBase::GetMaxEnergy()
{
	const float MaxEnergy = (EnergyMax + EnergyBoosted);
	return MaxEnergy;
}

float ACharacterBase::GetDataAsPercent()
{
	const float Percent = (DataCurrent / DataMax);
	return Percent;
}

void ACharacterBase::SavePlayerStats()
{
	if ( USaveGameBase* SaveGameInstance =
		Cast<USaveGameBase>( UGameplayStatics::CreateSaveGameObject(
			USaveGameBase::StaticClass()) ) )
	{
		SaveGameInstance->PlayerHealthMax = HealthMax;
		SaveGameInstance->PlayerHealthBoosted = HealthBoosted;
		SaveGameInstance->PlayerJumpCountMax = JumpCountMax;
		SaveGameInstance->PlayerJumpCountBoosted = JumpCountBoosted;

		FString SlotName = TEXT("PlayerStats");

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, 0);
	}
}

void ACharacterBase::LoadPlayerStats()
{
	FString SlotName = TEXT("PlayerStats");
	if ( USaveGameBase* SaveGameInstance =
		Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(SlotName, 0 )) )
	{
		HealthMax = SaveGameInstance->PlayerHealthMax;
		HealthBoosted = SaveGameInstance->PlayerHealthBoosted;
		JumpCountMax = SaveGameInstance->PlayerJumpCountMax;
		JumpCountBoosted = SaveGameInstance->PlayerJumpCountBoosted;
	}
}

//void ACharacterBase::OnPlayerStatsLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
//{
//}
