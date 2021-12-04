// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoundTrigger.h"
#include "ThirdRoundTrigger.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API AThirdRoundTrigger : public ARoundTrigger
{
	GENERATED_BODY()
	
public:
	virtual void BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
