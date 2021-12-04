// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerUMG.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API APlayerControllerUMG : public APlayerController
{
	GENERATED_BODY()
public:

	class UUserWidget* mainHUD;
	class UUserWidget* bossHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainHUD")
	TSubclassOf<class UUserWidget> subClassMainHUD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossHP")
	TSubclassOf<class UUserWidget> subClassBossHP;

	//class UGameInstance* gi;
	//class UGI* myGI;

	bool count;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
