// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdRoundTrigger.h"
#include "GI.h"
#include "MainPlayer.h"

void AThirdRoundTrigger::BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			bIsPlayCinematic = true;

			myGI->isBrokenThirdDoor = true;
		}
	}
}