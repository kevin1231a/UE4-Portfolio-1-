// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondRoundTrigger.h"
#include "GI.h"
#include "MainPlayer.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ASecondRoundTrigger::BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			bIsPlayCinematic = true;

			myGI->isBrokenSecondDoor = true;

			GetWorldTimerManager().SetTimer(timer, this, &ASecondRoundTrigger::PlayMentSec, 0.65f);
		}
	}
}

void ASecondRoundTrigger::PlayMentSec()
{
	UGameplayStatics::PlaySound2D(this, mentSoundSec);
}