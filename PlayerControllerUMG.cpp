// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerUMG.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayer.h"
#include "GI.h"

void APlayerControllerUMG::BeginPlay()
{
	Super::BeginPlay();

	if (subClassMainHUD)
	{
		mainHUD = CreateWidget<UUserWidget>(this, subClassMainHUD);
	}

	if (subClassBossHP)
	{
		bossHUD = CreateWidget<UUserWidget>(this, subClassBossHP);
	}

	mainHUD->AddToViewport();
	mainHUD->SetVisibility(ESlateVisibility::Visible);

	//gi = GetGameInstance();
	//myGI = Cast<UGI>(gi);

	count = false;
}

void APlayerControllerUMG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (myGI->isBossRound && !count)
	{
		count = true;

		bossHUD->AddToViewport();
		bossHUD->SetVisibility(ESlateVisibility::Visible);

		myGI->isBossRound = false;
	}*/
}