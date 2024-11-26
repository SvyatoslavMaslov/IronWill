// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	UE_LOG(LogTemp, Warning, TEXT("DamageAmount: %f"), DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("DamageToApply: %i"), DamageToApply);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		OnDeath.Broadcast();
	}
	return DamageToApply;
}

void ATank::BeginPlay() {
	Super::BeginPlay();
}
