// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine.h"

// Sets default values
AMine::AMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMine::BeginPlay()
{
	Super::BeginPlay();
	//CollisionMesh->SetVisibility(false);
}

// Called every frame
void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMine::OnTimerExpire()
{
	Destroy();
}

void AMine::HitHandler(AActor* Victim, UCapsuleComponent* Component)
{
	if (!(Victim || Component)) { return; }

	auto mass = Victim->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	if (mass < 200) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Mine BANG %f"), ExplosionForce->Radius);

	ExplosionForce->FireImpulse();
	ImpactBlast->Activate();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Cast<AActor>(CollisionMesh));
	UGameplayStatics::ApplyRadialDamage(GetWorld(), MineDamage, CollisionMesh->GetComponentLocation(), ExplosionForce->Radius, UDamageType::StaticClass(), IgnoreActors);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMine::OnTimerExpire, DestroyDelay, false);

	SetRootComponent(ImpactBlast);
	Component->Deactivate();
	CollisionMesh->DestroyComponent();
	Component->DestroyComponent();
}


