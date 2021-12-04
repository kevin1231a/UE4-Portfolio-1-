// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRangeAttackActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainPlayer.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossRangeAttackActor::ABossRangeAttackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	RootComponent = attackRangeSphere;
	explosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillParticle"));
	explosionParticle->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABossRangeAttackActor::BeginPlay()
{
	Super::BeginPlay();
	
	attackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossRangeAttackActor::BeginOverlapSkillRangeSphere);
}

// Called every frame
void ABossRangeAttackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossRangeAttackActor::BeginOverlapSkillRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		hitMainPlayer = Cast<AMainPlayer>(OtherActor);

		if (hitMainPlayer)
		{
			hitMainPlayer->PlayerStun();
		}
	}
}

void ABossRangeAttackActor::PlayExplosionSound()
{
	UGameplayStatics::PlaySound2D(this, explosionSound);
}