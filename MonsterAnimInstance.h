// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	class APawn* monsterPawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	class AMonster* monster;

	FVector velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float speed;

public:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
	void GetMonsterSpeed();
};
