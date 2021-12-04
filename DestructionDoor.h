// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructionDoor.generated.h"

UCLASS()
class FIRSTPORTFOLIORPG_API ADestructionDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructionDoor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StaticMesh")
	class UDestructibleComponent* doorDistructMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorBoxComponent")
	class UBoxComponent* doorTrigger;
	class AWeapon* swordWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorBreakParticle")
	class UParticleSystem* breakParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorHitParticle")
	class UParticleSystem* hitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorHitSound")
	class USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DoorBreakSound")
	class USoundCue* breakSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shake")
	TSubclassOf<UMatineeCameraShake> cameraShake;

	//class UGameInstance* gi;
	//class UGI* myGI;

	//class ULevelSequence* test;

	float doorHp;
	float damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsBreak")
	bool isBreak = false;

	FTimerHandle timer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DestroyDoor();
	void DoorCameraShake();
};