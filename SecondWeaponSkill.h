// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondWeaponSkill.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API ASecondWeaponSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondWeaponSkill();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillRangeSphere")
	class USphereComponent* skillRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillAura")
	class AMonster* hitMonster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillHitParticle")
	class UParticleSystem* skillHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondSkillSound")
	class USoundCue* skillSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillAura")
	class UParticleSystemComponent* skillAuraParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillAura")
	TArray<AMonster*> hitMonsterArray;

	FTimerHandle timerHandle;

	float skillDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillAura")
	bool bIsHitSecondSkill;
	float checkTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapSkillSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapSkillSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void HitSkill(float DeltaTime);
	void PlaySkillHitSound();
};