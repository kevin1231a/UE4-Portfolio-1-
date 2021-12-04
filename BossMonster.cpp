// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "AIController.h"
#include "MainPlayer.h"
#include "ProjectileEnegyBolt.h"
#include "Animation/AnimInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "BossSkillDecal.h"
#include "BossRangeAttackActor.h"
#include "GI.h"
#include "Components/WidgetComponent.h"
#include "PlayerControllerUMG.h"

ABossMonster::ABossMonster()
{
	movementCompo = GetCharacterMovement();

	movementCompo->MaxWalkSpeed = 450.f;

	skeletal = GetMesh();

	skillTimer = 0.f;
	randNum = 0;

	bIsStartFire = false;
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStartFire && !bIsInAttackRange && !chaseTargetPlayer->bIsUsingSkillTime)
	{
		skillTimer += DeltaTime;
		MakeRandomNum();
	}

	PlayFireEnergyBoltAnim();
	UseRangeAttack();
	PlayViewMotionBoss();
}

void ABossMonster::ChaseTarget()
{
	if (monsterStatusEnum == MonsterStatus::MS_Chase && !bIsAttack) //부모의 함수를 재구성했으니 부모의 tick에서 돌아간다, 여기서 Tick에 넣을 필요 없음
	{
		//gi = GetGameInstance();
		//myGI = Cast<UGI>(gi);

		//myGI->isBossRound = true;

		//부모의 chaseSphere에 닿자마자 한번 실행되기 때문에 중간에 상태를 바꿔도 멈추지 않는다
		bIsStartFire = true;

		SetBossMonster(this);

		FAIMoveRequest moveRequest;
		FNavPathSharedPtr navPath;

		moveRequest.SetGoalActor(chaseTargetPlayer);
		moveRequest.SetAcceptanceRadius(90.f);

		aiController->MoveTo(moveRequest, &navPath);
	}
	else
	{
		bIsStartFire = false;
	}
}

void ABossMonster::HitAnimation()
{
	if (!bIsUseSkill)
	{
		enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.0f);
		enemyAnimInstance->Montage_JumpToSection(FName("Hit"), enemyAnimMontage);
	}
}

void ABossMonster::StopDeathAnimation()
{
	skeletal->bPauseAnims = true;
}

void ABossMonster::FireEnergyBolt()
{
	UGameplayStatics::PlaySound2D(this, energyBoltShootSound);

	FVector energyBoltLoc = energyBoltActor->GetActorLocation();

	FVector targetLoc = chaseTargetPlayer->GetActorLocation();
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(energyBoltLoc, targetLoc);
	energyBoltActor->SetActorRotation(targetRot);

	energyBoltAuraCompo->Deactivate();

	energyBoltActor->projectileMovement->InitialSpeed = 3000.f;
	energyBoltActor->projectileMovement->MaxSpeed = 3000.f;

	energyBoltActor->FireEnergyBolt(energyBoltActor->GetActorForwardVector());

	GetWorldTimerManager().SetTimer(timerHandle, this, &ABossMonster::DestroyEnergyBolt, 3.f);
}

void ABossMonster::PlayFireEnergyBoltAnim()
{
	if (randNum == 0 && skillTimer >= 10.f)
	{
		SetBIsUseSKill();
		
		UGameplayStatics::PlaySound2D(this, energyBoltChargingSound);

		movementCompo->MaxWalkSpeed = 0.f;

		animInstance = skeletal->GetAnimInstance();

		animInstance->Montage_Play(bossAnimMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("FireEnergyBolt"), bossAnimMontage);

		energyBoltAuraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, energyBoltAura, GetActorLocation(), FRotator(0.f));

		UWorld* world = GetWorld();
		FActorSpawnParameters param;

		energyBoltActor = world->SpawnActor<AProjectileEnegyBolt>(energyBolt, GetActorLocation() + FVector(0.f, 0.f, 300.f), FRotator(0.f), param);

		skillTimer = 0.f;
	}
}

void ABossMonster::UseRangeAttack()
{
	if (randNum == 1 && skillTimer >= 10.f)
	{
		SetBIsUseSKill();

		movementCompo->MaxWalkSpeed = 0.f;

		animInstance = skeletal->GetAnimInstance();

		animInstance->Montage_Play(bossAnimMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("RangeAttack"), bossAnimMontage);

		UWorld* world = GetWorld();
		FActorSpawnParameters params;

		rangeSkillDecalActor = world->SpawnActor<ABossSkillDecal>(rangeSkillDecal, chaseTargetPlayer->GetActorLocation(), FRotator(0.f), params);

		if (rangeSkillDecalActor)
		{
			rangeSkillDecalActor->PlayDecalSound();
		}

		skillTimer = 0.f;
	}
}

void ABossMonster::SetMonsterStatus()
{
	movementCompo->MaxWalkSpeed = 450.f;
}

void ABossMonster::DestroyEnergyBolt()
{
	if (energyBoltActor)
	{
		energyBoltActor->Destroy();
	}
}

void ABossMonster::SpawnRangeAttack()
{
	UWorld* world = GetWorld();
	FActorSpawnParameters params;

	rangeSkillActor = world->SpawnActor<ABossRangeAttackActor>(rangeSkill, rangeSkillDecalActor->GetActorLocation() - FVector(0.f,0.f,90.f), FRotator(0.f), params);

	rangeSkillActor->PlayExplosionSound();

	GetWorldTimerManager().SetTimer(timerHandle, this, &ABossMonster::DestroySpawnRangeAttack, 3.f);
}

void ABossMonster::DestroySpawnRangeAttack()
{
	if (rangeSkillActor && rangeSkillDecalActor)
	{
		rangeSkillActor->Destroy();
		rangeSkillDecalActor->Destroy();
	}
}

void ABossMonster::MakeRandomNum()
{
	if (skillTimer >= 10.f)
	{
		randNum = FMath::RandRange(0, 1);
	}
}

void ABossMonster::SetBIsUseSKill()
{
	bIsUseSkill = !bIsUseSkill;
}

void ABossMonster::PlayViewMotionBoss()
{
	if (myGI->isBrokenThirdDoor)
	{
		animInstance = skeletal->GetAnimInstance();

		animInstance->Montage_Play(bossAnimMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("BossViewEmotion"), bossAnimMontage);

		myGI->isBrokenThirdDoor = false;
	}
}

void ABossMonster::PlayMent()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bossMentSound);
}