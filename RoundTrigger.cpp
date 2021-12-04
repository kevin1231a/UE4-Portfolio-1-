// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundTrigger.h"
#include "Components/BoxComponent.h"
#include "MainPlayer.h"
#include "GI.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ARoundTrigger::ARoundTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cinematicTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	cinematicTrigger->SetupAttachment(GetRootComponent());

	bIsPlayCinematic = false;
	bIsPlaySecCinematic = false;
	bIsPlayThirdCinematic = false;
}

// Called when the game starts or when spawned
void ARoundTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	cinematicTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARoundTrigger::BeginOverlapCinematicTrigger);
	cinematicTrigger->OnComponentEndOverlap.AddDynamic(this, &ARoundTrigger::EndOverlapinematicTrigger);

	gi = GetGameInstance();
	myGI = Cast<UGI>(gi);
}

// Called every frame
void ARoundTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoundTrigger::BeginOverlapCinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			bIsPlayCinematic = true;

			myGI->isBrokenFirstDoor = true;

			GetWorldTimerManager().SetTimer(timer, this, &ARoundTrigger::PlayMent, 0.65f);
		}
	}
}

void ARoundTrigger::EndOverlapinematicTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			cinematicTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ARoundTrigger::PlayMent()
{
	UGameplayStatics::PlaySound2D(this, mentSound);
}