// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "SecondSword.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API ASecondSword : public AWeapon
{
	GENERATED_BODY()
	
public:
	ASecondSword();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CSkillHitSound");
	class USoundCue* cSkillHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CSkillHitParticle")
	class UParticleSystem* secondCSkillHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireSwordNiagara")
	class UNiagaraComponent* fireSwordNiagara;
};