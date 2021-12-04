// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API UGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	bool isBrokenFirstDoor = false;
	bool isBrokenSecondDoor = false;
	bool isBrokenThirdDoor = false;

	//bool isBossRound = false;
};
