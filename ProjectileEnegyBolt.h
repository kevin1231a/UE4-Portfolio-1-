// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileEnegyBolt.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API AProjectileEnegyBolt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileEnegyBolt();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CollisionSphere")
	class USphereComponent* collisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyParticleComponent")
	class UParticleSystemComponent* energyParticle;
	class AMainPlayer* hitMainPlayer;
	class UProjectileMovementComponent* projectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChargingParticle")
	class UParticleSystemComponent* chargingParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyBoltHitSound")
	class USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitParticle")
	class UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float energyBoltDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapCollisionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void FireEnergyBolt(FVector targetVector);
};
