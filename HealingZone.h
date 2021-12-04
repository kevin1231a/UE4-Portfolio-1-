// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingZone.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API AHealingZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingZone();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SphereCollider")
	class USphereComponent* sphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IdleParticle")
	class UParticleSystemComponent* idleParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealParticle")
	class UParticleSystemComponent* healParticle;
	class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealSound")
	class USoundCue* healSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AudioComponent")
	class UAudioComponent* audioComponent;

	bool bIsOnHealingZone;
	const float tickHealValue = 5.f;
	const float tickMpValue = 5.f;
	const float playerMaxHp = 200.f;
	const float playerMaxMp = 100.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapHealingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapHealingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void HealPlayer(float delta);
};
