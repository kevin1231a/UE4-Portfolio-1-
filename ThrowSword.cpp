// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowSword.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"
#include "Components/PrimitiveComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AThrowSword::AThrowSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	visualParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VisualParticle"));
	RootComponent = visualParticle;
	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	attackRangeSphere->SetupAttachment(visualParticle);
	skillEffectAura = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillEffectNiagara"));
	skillEffectAura->SetupAttachment(GetRootComponent());
	drainSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DrainSphere"));
	drainSphere->SetupAttachment(GetRootComponent());

	deltaCalc = 0;
	skillDamage = 50.f;
	hitLoc = FVector(0.f);

	bIsCollectEnemy = false;

	drainSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AThrowSword::BeginPlay()
{
	Super::BeginPlay();
	
	attackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AThrowSword::BeginOverlapSpinSwordSphere);
	attackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AThrowSword::EndOverlapSpinSwordSphere);
	drainSphere->OnComponentBeginOverlap.AddDynamic(this, &AThrowSword::BeginOverlapDrainSphere);
}

// Called every frame
void AThrowSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHitThrowSword)
	{
		deltaCalc += DeltaTime;

		ThrowSwordAttack(DeltaTime);
	}

	if (bIsCollectEnemy)
	{
		CollectEnemy(DeltaTime);
	}
}

void AThrowSword::BeginOverlapSpinSwordSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		hitMonster = Cast<AMonster>(OtherActor);

		if (hitMonster)
		{
			UE_LOG(LogTemp, Warning, TEXT("dd"));
			drainSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			bIsHitThrowSword = true;
		}
	}
}

void AThrowSword::EndOverlapSpinSwordSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		hitMonster = Cast<AMonster>(OtherActor);

		if (hitMonster)
		{
			bIsCollectEnemy = false;

			bIsHitThrowSword = false;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorLocation(), FRotator(0.f), true);

			skillEffectAura->Deactivate();
		}
	}
}

void AThrowSword::ThrowSwordAttack(float DeltaTime)
{
	if (deltaCalc >= 0.25f && hitmonsterArray[0])
	{
		deltaCalc = 0.f;

		UGameplayStatics::PlaySound2D(this, hitSound);

		for (int i = 0; i < hitmonsterArray.Num(); i++)
		{
			if (hitmonsterArray[i]->monsterHp - skillDamage > 0.f)
			{
				hitmonsterArray[i]->monsterHp -= skillDamage;
				hitmonsterArray[i]->HitAnimation();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, hitmonsterArray[i]->GetActorLocation(), FRotator(0.f), true);
			}
			else
			{
				hitmonsterArray[i]->MonsterDie();
			}
		}
	}
}

void AThrowSword::CollectEnemy(float DeltaTime)
{
	for (int i = 0; i < hitmonsterArray.Num(); i++)
	{
		hitMonsterLocArray.Add(hitmonsterArray[i]->GetActorLocation());

		hitMonsterLocArray[i] = FMath::VInterpTo(hitMonsterLocArray[i], skillLocation, DeltaTime, 5.f);
		hitmonsterArray[i]->SetActorLocation(hitMonsterLocArray[i]);
	}
}

void AThrowSword::BeginOverlapDrainSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		hitMonster = Cast<AMonster>(OtherActor);

		if (hitMonster)
		{
			hitmonsterArray.Add(hitMonster);

			skillLocation = GetActorLocation();

			bIsCollectEnemy = true;

			skillEffectAura->Activate();

			if (mainPlayer)
			{
				mainPlayer->bIsThrowSword = false;
			}

			bIsHitThrowSword = true;
		}
	}
}