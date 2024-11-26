// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::Rotate(float DeltaRotation) {
	auto Direction = FMath::Clamp<float>(DeltaRotation, -1, 1);
	auto RotationChange = Direction * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewYaw = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0, NewYaw, 0));
}
