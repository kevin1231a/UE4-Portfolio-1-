// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowSword.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API AThrowSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowSword();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeSphere")
	class USphereComponent* attackRangeSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VisualParticle")
	class UParticleSystemComponent* visualParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitMonster")
	class AMonster* hitMonster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VisualParticle")
	class UParticleSystem* hitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VisualParticle")
	class UParticleSystem* explosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VisualParticle")
	class UNiagaraComponent* skillEffectAura;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitSound")
	class USoundCue* hitSound;
	class AMainPlayer* mainPlayer;
	FORCEINLINE void SetMainPlayer(AMainPlayer* main) { mainPlayer = main; }
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrainSphere")
	class USphereComponent* drainSphere;

	bool bIsHitThrowSword; 
	float deltaCalc;
	float skillDamage;
	FVector hitLoc;
	bool bIsCollectEnemy;
	FVector skillLocation;
	
	TArray<FVector> hitMonsterLocArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitMonsterArray")
	TArray<AMonster*> hitmonsterArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapSpinSwordSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapSpinSwordSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void ThrowSwordAttack(float DeltaTime);
	void CollectEnemy(float DeltaTime);
	UFUNCTION()
	void BeginOverlapDrainSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};