// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

UENUM()
enum class EFiringState: uint8 {
	Reloading,
	Aiming,
	Locked,
	No_Projectile,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IRONWILL_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAimingComponent();

	void BeginPlay();
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	bool IsBarrelMoving();

	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 10000;

	virtual void AimAt(FVector HitLocation);

	EFiringState GetFiringState() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EFiringState FiringState = EFiringState::Aiming;

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTimeInSeconds = 3;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetProjectileAmount() const;

	double LastFireTime = 0;
private:	
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	virtual void MoveBarrel(FVector AimDirection);
	virtual void MoveTurret(FVector AimDirection);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet);

	FVector AimDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 ProjectileAmount = 30;
};
