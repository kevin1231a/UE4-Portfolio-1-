// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */

UCLASS()
class FIRSTPORTFOLIORPG_API ABossMonster : public AMonster
{
	GENERATED_BODY()
	
public:
	ABossMonster();

	class UCharacterMovementComponent* movementCompo;
	class USkeletalMeshComponent* skeletal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyBolt")
	TSubclassOf<class AProjectileEnegyBolt> energyBolt;
	class AProjectileEnegyBolt* energyBoltActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossAnimMontage")
	class UAnimMontage* bossAnimMontage;
	class UAnimInstance* animInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyBoltAura")
	class UNiagaraSystem* energyBoltAura;
	class UNiagaraComponent* energyBoltAuraCompo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyBoltCharging")
	class USoundCue* energyBoltChargingSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnergyBoltShooting")
	class USoundCue* energyBoltShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeSkillDecal")
	TSubclassOf<class ABossSkillDecal> rangeSkillDecal;
	class ABossSkillDecal* rangeSkillDecalActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeSkillActor")
	TSubclassOf<class ABossRangeAttackActor> rangeSkill;
	class ABossRangeAttackActor* rangeSkillActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeSkillActor")
	class USoundCue* bossMentSound;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TESTUMG")
	class APlayerControllerUMG* testUMG;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TESTUMG")
	TSubclassOf<class APlayerControllerUMG> playerumg;*/

	//class UGameInstance* gi;
	//class UGI* myGI;

	float skillTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IsStartFire")
	bool bIsStartFire;
	FTimerHandle timerHandle;
	int randNum;

public:
	virtual void ChaseTarget() override;
	virtual void HitAnimation() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void StopDeathAnimation();
	UFUNCTION(BlueprintCallable)
	void FireEnergyBolt();
	void PlayFireEnergyBoltAnim();
	UFUNCTION(BlueprintCallable)
	void SetMonsterStatus();
	void DestroyEnergyBolt();
	void UseRangeAttack();
	UFUNCTION(BlueprintCallable)
	void SpawnRangeAttack();
	void DestroySpawnRangeAttack();
	void MakeRandomNum();
	UFUNCTION(BlueprintCallable)
	void SetBIsUseSKill();
	void PlayViewMotionBoss();
	UFUNCTION(BlueprintCallable)
	void PlayMent();
};
