// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FirstSword.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API AFirstSword : public AWeapon
{
	GENERATED_BODY()

public:
	AFirstSword();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChargingParticle")
	class UParticleSystemComponent* skillChargingParticle;

public:
	UFUNCTION(BlueprintCallable)
	void TurnOnChargingParticle();
	UFUNCTION(BlueprintCallable)
	void TurnOffChargingParticle();
	virtual void BeginPlay() override;
};
