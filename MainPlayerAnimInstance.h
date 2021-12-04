// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPORTFOLIORPG_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPawn")
	class APawn* mainPlayerPawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPawn")
	class AMainPlayer* mainPlayer;

	FVector velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPawn")
	float speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPawn")
	bool bIsJump;

public:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
	void GetPlayerSpeed();
};