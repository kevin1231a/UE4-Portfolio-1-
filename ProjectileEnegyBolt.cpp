// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEnegyBolt.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainPlayer.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileEnegyBolt::AProjectileEnegyBolt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = collisionSphere;
	energyParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyBolt"));
	energyParticle->SetupAttachment(collisionSphere);
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	chargingParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Charging"));
	chargingParticle->SetupAttachment(collisionSphere);

	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->ProjectileGravityScale = 0.f;

	energyBoltDamage = 50.f;
}

// Called when the game starts or when spawned
void AProjectileEnegyBolt::BeginPlay()
{
	Super::BeginPlay();
	
	projectileMovement->InitialSpeed = 0.f;
	projectileMovement->MaxSpeed = 0.f;

	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileEnegyBolt::BeginOverlapCollisionSphere);
}

// Called every frame
void AProjectileEnegyBolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileEnegyBolt::BeginOverlapCollisionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		hitMainPlayer = Cast<AMainPlayer>(OtherActor);

		if (hitMainPlayer)
		{
			hitMainPlayer->CalcDamage(energyBoltDamage);
			hitMainPlayer->PlayerStun();

			UGameplayStatics::PlaySound2D(this, hitSound);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, hitMainPlayer->GetActorLocation() - FVector(0.f, 0.f, 100.f), FRotator(0.f), true);

			this->Destroy();
		}
	}
}

void AProjectileEnegyBolt::FireEnergyBolt(FVector targetVector)
{
	projectileMovement->Velocity = targetVector * projectileMovement->InitialSpeed;
}