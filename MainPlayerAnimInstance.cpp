// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnimInstance.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "MainPlayer.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	if (mainPlayerPawn == nullptr)
	{
		mainPlayerPawn = TryGetPawnOwner();

		mainPlayer = Cast<AMainPlayer>(mainPlayerPawn);
	}
	if (mainPlayer)
	{
		velocity = mainPlayerPawn->GetVelocity();
		speed = velocity.Size();

		bIsJump = mainPlayerPawn->GetMovementComponent()->IsFalling();
	}
}

void UMainPlayerAnimInstance::GetPlayerSpeed()
{
	if (mainPlayerPawn == nullptr)
	{
		mainPlayerPawn = TryGetPawnOwner();

		mainPlayer = Cast<AMainPlayer>(mainPlayerPawn);
	}
	if (mainPlayer)
	{
		velocity = mainPlayerPawn->GetVelocity();
		speed = velocity.Size();

		bIsJump = mainPlayerPawn->GetMovementComponent()->IsFalling();
	}
}