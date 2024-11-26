// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "AimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) {
			return;
		}

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (GetPawn()) {
		GetPawn()->DetachFromControllerPendingDestroy();
	}

}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto AimingComponent = GetPawn()->FindComponentByClass<UAimingComponent>();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (PlayerTank && AimingComponent) {
		MoveToActor(PlayerTank, AcceptanceRadius);
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringState::Locked) {
			AimingComponent->Fire();
		}
	}
}
