// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructionDoor.h"
#include "Components/StaticMeshComponent.h"
#include "DestructibleComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GI.h"

// Sets default values
ADestructionDoor::ADestructionDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	doorDistructMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Door"));
	doorDistructMesh->SetupAttachment(RootComponent);
	doorDistructMesh->SetNotifyRigidBodyCollision(true);

	doorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	doorTrigger->SetupAttachment(RootComponent);

	doorHp = 100.f;
	damage = 50.f;
}

// Called when the game starts or when spawned
void ADestructionDoor::BeginPlay()
{
	Super::BeginPlay();

	doorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADestructionDoor::Trigger);

	//gi = GetGameInstance();
	//myGI = Cast<UGI>(gi);
}

// Called every frame
void ADestructionDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructionDoor::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		swordWeapon = Cast<AWeapon>(OtherActor);

		if (swordWeapon)
		{
			if (doorHp - damage > 0)
			{
				UGameplayStatics::PlaySound2D(this, hitSound);
				
				doorHp -= damage;

				DoorCameraShake();
			}
			else
			{
				UGameplayStatics::PlaySound2D(this, breakSound);

				doorDistructMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				DoorCameraShake();

				//isBreak = true;
				
				//myGI->isBrokenFirstDoor = true;

				doorDistructMesh->SetSimulatePhysics(true);

				doorHp = 0.f;

				GetWorldTimerManager().SetTimer(timer, this, &ADestructionDoor::DestroyDoor, 3);
			}
		}
	}
}

void ADestructionDoor::DestroyDoor()
{
	this->Destroy();
}

void ADestructionDoor::DoorCameraShake()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(cameraShake, 1.0f);
}