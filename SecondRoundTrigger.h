// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoundTrigger.h"
#include "SecondRoundTrigger.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API ASecondRoundTrigger : public ARoundTrigger
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecMentSound")
	class USoundCue* mentSoundSec;

public:
	virtual void BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void PlayMentSec();
};
