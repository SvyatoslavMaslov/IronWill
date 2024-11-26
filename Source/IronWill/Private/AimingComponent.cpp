// Fill out your copyright notice in the Description page of Project Settings.

#include "AimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAimingComponent::UAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UAimingComponent::BeginPlay() {
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
}

void UAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (ProjectileAmount <= 0) {
		FiringState = EFiringState::No_Projectile;
	} else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::Aiming;
	} else {
		FiringState = EFiringState::Locked;
	}
}

void UAimingComponent::Initialise(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet) {
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}

void UAimingComponent::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) {
		return;
	}

	FVector OutLaucnVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	auto HaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaucnVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	/*
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		HitLocation,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
	*/

	/*
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerPointView.Location,
		GetReachLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	*/

	if (HaveAimSolution) {
		AimDirection = OutLaucnVelocity.GetSafeNormal();
		MoveBarrel(AimDirection);
		MoveTurret(AimDirection);
	}
}

EFiringState UAimingComponent::GetFiringState() const
{
	return FiringState;
}

bool UAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) {
		return false;
	}

	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01);
}

int32 UAimingComponent::GetProjectileAmount() const
{
	return ProjectileAmount;
}

void UAimingComponent::MoveBarrel(FVector AimDirection)
{
	if (!ensure(Turret)) {
		return;
	}

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//UE_LOG(LogTemp, Warning, TEXT("DeltaRotator Pitch: %f"), DeltaRotator.Pitch);
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UAimingComponent::MoveTurret(FVector AimDirection)
{
	if (!ensure(Turret)) {
		return;
	}

	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotator;

	Turret->Rotate(FMath::Abs(DeltaRotator.Yaw) < 180 ? DeltaRotator.Yaw: -DeltaRotator.Yaw);
}

void UAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) {
		return;
	}

	if (ProjectileAmount <= 0) {
		return;
	}

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
		);

		Projectile->Lauch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		--ProjectileAmount;
	}
}
