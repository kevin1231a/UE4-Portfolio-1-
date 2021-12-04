// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSkillDecal.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossSkillDecal::ABossSkillDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossSkillDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossSkillDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossSkillDecal::PlayDecalSound()
{
	UGameplayStatics::PlaySound2D(this, decalSound);
}