// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossRangeAttackActor.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API ABossRangeAttackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRangeAttackActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeSphere")
	class USphereComponent* attackRangeSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionParticle")
	class UParticleSystemComponent* explosionParticle;
	class AMainPlayer* hitMainPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionSound")
	class USoundCue* explosionSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapSkillRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PlayExplosionSound();
};
