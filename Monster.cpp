// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "MainPlayer.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "SecondWeaponSkill.h"
#include "Components/WidgetComponent.h"
#include "DestructionDoor.h"
#include "GI.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chaseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseSphere"));
	chaseSphere->SetupAttachment(GetRootComponent());
	enemyWeaponSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyWeapon"));
	enemyAttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	attackRangeSphere->SetupAttachment(GetRootComponent());
	hpUMG = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpWidget"));
	hpUMG->SetupAttachment(GetRootComponent());

	chaseSphere->SetSphereRadius(500.f);

	hitCapsule = GetCapsuleComponent();
	
	hitCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	monsterMovementComponent = GetCharacterMovement();
	enemySkeletalMesh = GetMesh();

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bIsInAttackRange = false;
	bIsAttack = false;
	bCanChase = true;
	bCanAttack = true;
	bIsFallingByHit = false;
	bIsUseSkill = false;
	bIsTurnOnHpBar = false;

	monsterMovementComponent->bOrientRotationToMovement = true;

	enemyWeaponSkeletal->AttachToComponent(enemySkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemyWeaponSocket"));
	enemyAttackBox->AttachToComponent(enemySkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemyWeaponSocket"));

	monsterStatusEnum = MonsterStatus::MS_Idle;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	chaseSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginOverlapChaseSphere);
	chaseSphere->OnComponentEndOverlap.AddDynamic(this, &AMonster::EndOverlapChaseSphere);
	attackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginOverlapAttackRangeSphere);
	attackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AMonster::EndOverlapAttackRangeSphere);
	enemyAttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginOverlapAttackBox);

	aiController = Cast<AAIController>(GetController());	
	enemyAnimInstance = enemySkeletalMesh->GetAnimInstance();

	enemyAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	chaseTargetActor = UGameplayStatics::GetActorOfClass(GetWorld(), subclassMainPlayer);
	chaseTargetPlayer = Cast<AMainPlayer>(chaseTargetActor);

	gi = GetGameInstance();
	myGI = Cast<UGI>(gi);

	hpUMG->SetHiddenInGame(true);

}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanChase)
	{
		ChaseTarget();
	}

	if (bIsFallingByHit)
	{
		CheckIsOnGround();
	}

	PlayViewEmotion();
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::BeginOverlapChaseSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		targetPlayer = Cast<AMainPlayer>(OtherActor);

		if (targetPlayer)
		{
			hpUMG->SetHiddenInGame(false);

			bIsTurnOnHpBar = true;

			chaseTargetPlayer = targetPlayer;

			SetMonsterStatus(MonsterStatus::MS_Chase);

			targetPlayer->SetAutoAimMonster(this);

			ChaseTarget();
		}
	}
}

void AMonster::EndOverlapChaseSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		targetPlayer = Cast<AMainPlayer>(OtherActor);

		if (targetPlayer)
		{
			SetMonsterStatus(MonsterStatus::MS_Idle);
			
			targetPlayer->SetAutoAimMonster(nullptr);

			aiController->StopMovement();
		}
	}
}

void AMonster::MonsterDie()
{
	enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.f);
	enemyAnimInstance->Montage_JumpToSection(FName("Die"), enemyAnimMontage);

	enemyAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	hitCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	GetWorldTimerManager().SetTimer(timerHandle, this, &AMonster::MonsterDestroyTimer, 1.5f);

	SetMonsterStatus(MonsterStatus::MS_Die);
}

void AMonster::MonsterHitByPlayer(float weaponDamage)
{
	if (monsterHp - weaponDamage > 0.f)
	{
		monsterHp -= weaponDamage;

		HitAnimation();
	}
	else if (monsterHp - weaponDamage <= 0.f)
	{		
		monsterHp = 0.f;

		MonsterDie();
	}
}

void AMonster::MonsterDestroyTimer()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), monsterDieParticle, GetActorLocation() - dieParticleLoc, FRotator(0.f), true);

	this->Destroy();
}

void AMonster::BeginOverlapAttackRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		targetPlayer = Cast<AMainPlayer>(OtherActor);

		if (targetPlayer)
		{
			if (bCanAttack)
			{
				SetMonsterStatus(MonsterStatus::MS_Attack);

				bIsInAttackRange = true;

				MonsterAttackLoop();
			}
		}
	}
}

void AMonster::EndOverlapAttackRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		targetPlayer = Cast<AMainPlayer>(OtherActor);

		if (targetPlayer)
		{
			SetMonsterStatus(MonsterStatus::MS_Chase);

			bIsInAttackRange = false;
		}
	}
}

void AMonster::MonsterAttackLoop()
{
	if (bIsInAttackRange && !bIsUseSkill)
	{
		enemyAnimInstance->Montage_Play(enemyAnimMontage, 0.6f);
		enemyAnimInstance->Montage_JumpToSection(FName("Attack"), enemyAnimMontage);
	}
}

void AMonster::ChaseTarget()
{
	if (monsterStatusEnum == MonsterStatus::MS_Chase && !bIsAttack)
	{
		FAIMoveRequest moveRequest;
		FNavPathSharedPtr navPath;

		moveRequest.SetGoalActor(chaseTargetPlayer);
		moveRequest.SetAcceptanceRadius(10.f);

		aiController->MoveTo(moveRequest, &navPath);
	}
}

void AMonster::HitAnimation()
{
	enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.5f);
	enemyAnimInstance->Montage_JumpToSection(FName("Hit"), enemyAnimMontage);
}

void AMonster::BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		targetPlayer = Cast<AMainPlayer>(OtherActor);

		if (targetPlayer)
		{
			UGameplayStatics::ApplyDamage(targetPlayer, monsterDamage, aiController, this, damageType);

			targetPlayer->SpawnHitParticle();
		}
	}
}

void AMonster::AttackBoxCollisionOn()
{
	enemyAttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMonster::AttackBoxCollisionOff()
{
	enemyAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonster::SecondSkillLaunch()
{
	this->LaunchCharacter(FVector(0.f, 0.f, 300.f), true, true);
}

void AMonster::SecondSkillLastLaunch()
{
	FVector enemyForward = GetActorForwardVector();

	this->LaunchCharacter(enemyForward * FVector(-700.f, 0.f, 900.f), true, true);
}

void AMonster::PlayHitBySkillLastAnim()
{
	bCanChase = false;
	bCanAttack = false;
	bIsFallingByHit = true;

	SetMonsterStatus(MonsterStatus::MS_Idle);

	enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.0f);
	enemyAnimInstance->Montage_JumpToSection(FName("HitBySkill"), enemyAnimMontage);

	FVector playerForward = chaseTargetPlayer->GetActorForwardVector();

	chaseSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->LaunchCharacter(FVector(chaseTargetPlayer->forward.X, 0.f, 1.f) * FVector(1300.f, 0.f, -500.f), true, true);
}

void AMonster::SetCanChase()
{
	chaseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bCanChase = true;

	SetMonsterStatus(MonsterStatus::MS_Chase);
}

void AMonster::SetCanAttack()
{
	bCanAttack = true;
}

void AMonster::CheckIsOnGround()
{
	FHitResult hit;
	FCollisionQueryParams params;

	FVector start = GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z - 82.f);

	GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility, params);

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor::Blue, 10.f, 10.f);

	if (hit.GetActor())
	{		
		bIsFallingByHit = false;
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), onGroundParticle, GetActorLocation(), FRotator(0.f), true);
	}
}

void AMonster::PlayViewEmotion()
{
	if (myGI->isBrokenFirstDoor)
	{
		enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.f);
		enemyAnimInstance->Montage_JumpToSection(FName("ViewEmotion"), enemyAnimMontage);

		myGI->isBrokenFirstDoor = false;
	}
	else if (myGI->isBrokenSecondDoor)
	{
		enemyAnimInstance->Montage_Play(enemyAnimMontage, 1.f);
		enemyAnimInstance->Montage_JumpToSection(FName("ViewEmotionSecond"), enemyAnimMontage);

		myGI->isBrokenSecondDoor = false;
	}
}