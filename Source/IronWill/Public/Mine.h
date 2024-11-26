// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mine.generated.h"

UCLASS()
class IRONWILL_API AMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMine();

	UFUNCTION(BlueprintCallable)
	void HitHandler(AActor* Victim, UCapsuleComponent* Component);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnTimerExpire();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float  DestroyDelay = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float  MineDamage = 20.f;
};
