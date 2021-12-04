// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Monster.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	if (monsterPawn == nullptr)
	{
		monsterPawn = TryGetPawnOwner();

		monster = Cast<AMonster>(monsterPawn);
	}
	if (monster)
	{
		velocity = monsterPawn->GetVelocity();
		speed = velocity.Size();
	}
}

void UMonsterAnimInstance::GetMonsterSpeed()
{
	if (monsterPawn == nullptr)
	{
		monsterPawn = TryGetPawnOwner();

		monster = Cast<AMonster>(monsterPawn);
	}
	if (monster)
	{
		velocity = monsterPawn->GetVelocity();
		speed = velocity.Size();
	}
}