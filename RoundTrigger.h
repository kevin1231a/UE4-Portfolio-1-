// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundTrigger.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API ARoundTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* cinematicTrigger;
	class AMainPlayer* mainPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsPlayCinematic")
	bool bIsPlayCinematic;
	bool bIsPlaySecCinematic;
	bool bIsPlayThirdCinematic;

	class UGameInstance* gi;
	class UGI* myGI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstMentSound")
	class USoundCue* mentSound;

	FTimerHandle timer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlapinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void PlayMent();
};