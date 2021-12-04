// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingZone.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainPlayer.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AHealingZone::AHealingZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	idleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	healParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HealParticle"));
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioCompo"));

	sphereCollision->SetupAttachment(RootComponent);
	idleParticle->SetupAttachment(RootComponent);
	healParticle->SetupAttachment(RootComponent);
	audioComponent->SetupAttachment(RootComponent);

	bIsOnHealingZone = false;


}

// Called when the game starts or when spawned
void AHealingZone::BeginPlay()
{
	Super::BeginPlay();
	
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealingZone::BeginOverlapHealingZone);
	sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AHealingZone::EndOverlapHealingZone);
}

// Called every frame
void AHealingZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealPlayer(DeltaTime);
}

void AHealingZone::BeginOverlapHealingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			bIsOnHealingZone = true;

			healParticle->Activate();

			audioComponent->SetSound(healSound);
			audioComponent->Play();
			//UGameplayStatics::PlaySound2D(this, healSound);
			
		}
	}
}

void AHealingZone::EndOverlapHealingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			bIsOnHealingZone = false;

			audioComponent->Stop();

			healParticle->Deactivate();
		}
	}
}

void AHealingZone::HealPlayer(float delta)
{
	if (bIsOnHealingZone)
	{
		if (mainPlayer->playerHp + delta * tickHealValue <= playerMaxHp)
		{
			mainPlayer->playerHp += delta * tickHealValue;
		}
		else
		{
			mainPlayer->playerHp = playerMaxHp;
		}
		
		if (mainPlayer->playerMp + delta * tickHealValue <= playerMaxMp)
		{
			mainPlayer->playerMp += delta * tickMpValue;
		}
		else
		{
			mainPlayer->playerMp = playerMaxMp;
		}
	}
}