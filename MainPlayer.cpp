// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "SecondWeaponSkill.h"
#include "SecondSword.h"
#include "ThrowSword.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/PostProcessVolume.h"
#include "ThirdCSkillDecal.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	dashParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashParticleComponent"));
	SkillCameraLocationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SkillCameraLocationBox"));
	SkillCameraOriginLocationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SkillCameraOriginLocationBox"));
	secondCSkillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CSkillSphereCompo"));
	stunParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StunStar"));

	springArm->SetupAttachment(GetRootComponent());
	mainCamera->SetupAttachment(springArm);
	dashParticle->SetupAttachment(GetRootComponent());
	SkillCameraLocationBox->SetupAttachment(GetRootComponent());
	SkillCameraOriginLocationBox->SetupAttachment(GetRootComponent());
	secondCSkillSphere->SetupAttachment(GetRootComponent());
	stunParticle->SetupAttachment(GetRootComponent());

	springArm->TargetArmLength = 550.f;
	springArm->SetWorldRotation(FRotator(-30.f, 0.f, 0.f));
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 10.f;
	springArm->bUsePawnControlRotation = true;

	characterMovement = GetCharacterMovement();
	playerSkeletal = this->GetMesh();
	playerCapsuleComponent = GetCapsuleComponent();

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.f, 540.f, 0.f);

	bIsSprint = false;
	bCanAttack = true;
	bIsDead = false;
	bCanMove = true;
	bIsDash = false;
	bIsEquip = false;
	bIsUsingSkill = false;
	bIsUsingThirdSkill = false;
	bIsUsingSecondCSkill = false;
	bCanMoveMouse = true;
	bIsUseCSkill = false;
	cSkillCanMove = true;
	bIsThrowSword = false;
	bIsWalkFront = false;
	bFirstPlayWalkSound = false;
	bFirstPlayWalkSound1 = false;
	bIsChargingSecondR = false;
	bIsUsingSkillTime = false;
	isCoolTimeR = false;
	isCoolTimeC = false;

	walkSpeed = 600.f;
	sprintSpeed = 750.f;
	comboTime = 0.f;
	orignCameraZ = 260.f;
	comboHeight = 80.f;
	dashDistance = 0.f;
	dashGoalDistance = 44.f;
	armlength = 550.f;
	firstRSkillDamage = 200.f;
	weaponSwapNum = 0;
	cSkillMoveCount = 0;
	firstCSkillDamage = 40.f;
	cSkillSpringArmLength = 400.f;
	SwordMoveRange = 0.f;
	targetMoveRange1 = 900.f;
	walkForwardValue = 0.f;
	walkRightValue = 0.f;
	moveSoundValue = 40.f;
	playerMp = 100.f;

	rSkillCoolTime = 0.f;
	cSkillCoolTime = 0.f;

	equipStatus = PlayerEquipStatus::PES_NoWeapon;
	attackStatus = PlayerAttackStatus::PAS_FirstAttack;
	usingCombo = PlayerUsingCombo::PUC_NoCombo;
	usingSkill = PlayerUsingSkill::PUC_NoSkill;

	secondCSkillSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
		
	secondCSkillSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::BeginOverlapSecondCSkill);

	equipSocket = playerSkeletal->GetSocketByName("SwordEquipSocket");
	weaponKeepSocket1 = playerSkeletal->GetSocketByName("WeaponKeepSocket1");
	weaponKeepSocket2 = playerSkeletal->GetSocketByName("WeaponKeepSocket2");

	playerAnimInstance = playerSkeletal->GetAnimInstance();

	dashParticle->Deactivate();

	equipWeapon = PlayerEquipWeapon::PEW_NoWeapon;

	firstCameraLoc = mainCamera->GetComponentLocation();
	firstCameraRot = mainCamera->GetComponentRotation();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalcComboTime(DeltaTime);

	if (usingCombo == PlayerUsingCombo::PUC_UpCombo)
	{
		ComboSpringArmLocUp(DeltaTime);
	}
	else if (usingCombo == PlayerUsingCombo::PUC_DownCombo)
	{
		ComboSpringArmLocDown(DeltaTime);
	}

	if (bIsDash)
	{
		PlayerDash(DeltaTime);
	}

	if (usingSkill == PlayerUsingSkill::PUC_SkillStart && equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
	{
		SkillCameraMove(DeltaTime);
	}
	else if (usingSkill == PlayerUsingSkill::PUC_SkillEnd && equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
	{
		SkillCameraMoveEnd(DeltaTime);
	}
	else if (usingSkill == PlayerUsingSkill::PUC_SkillStart && equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
	{
		SkillCameraMove2(DeltaTime);
	}
	else if (bIsChargingSecondR)
	{
		SkillCameraMove2ChargingR(DeltaTime);
	}
	else if (usingSkill == PlayerUsingSkill::PUC_SkillEnd && equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
	{
		SkillCameraMoveEnd2(DeltaTime);

		if (springArm->TargetArmLength >= 680.f && springArm->TargetArmLength <= 800.f)
		{
			springArm->TargetArmLength = 700.f;

			SetUsingSkill(PlayerUsingSkill::PUC_NoSkill);
		} 
	}

	if (bIsUsingSecondCSkill)
	{
		SecondCSkill(DeltaTime);
	}

	if (bIsUseCSkill)
	{
		CSkillCameraMove(DeltaTime);
	}

	if (bIsThrowSword)
	{
		MoveThrowSword(DeltaTime);
	}

	if (bIsSprint)
	{
		moveSoundValue = 30.f;
	}
	else if(!bIsSprint)
	{
		moveSoundValue = 40.f;
	}

	CalcRSkillCoolTime(DeltaTime);
	CalcCSkillCoolTime(DeltaTime);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)	
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MouseUp"), this, &AMainPlayer::MouseUpFunc);
	PlayerInputComponent->BindAxis(TEXT("MouseRight"), this, &AMainPlayer::MouseRightFunc);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMainPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMainPlayer::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &AMainPlayer::InputEquipKey);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMainPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AMainPlayer::SetIsDash);
	PlayerInputComponent->BindAction(TEXT("RSkill"), IE_Pressed, this, &AMainPlayer::RSkill);
	PlayerInputComponent->BindAction(TEXT("CSkill"), IE_Pressed, this, &AMainPlayer::UseCSkill);
	PlayerInputComponent->BindAction(TEXT("WeaponSwap"), IE_Pressed, this, &AMainPlayer::WeaponSwap);
}

void AMainPlayer::CalcRSkillCoolTime(float DeltaTime)
{
	if (isCoolTimeR)
	{
		rSkillCoolTime -= DeltaTime;
	}

	if (rSkillCoolTime <= 0.f)
	{
		rSkillCoolTime = 0.f;

		isCoolTimeR = false;
	}
}

void AMainPlayer::CalcCSkillCoolTime(float DeltaTime)
{
	if (isCoolTimeC)
	{
		cSkillCoolTime -= DeltaTime;
	}

	if (cSkillCoolTime <= 0.f)
	{
		cSkillCoolTime = 0.f;

		isCoolTimeC = false;
	}
}

void AMainPlayer::MouseUpFunc(float value)
{
	if (bCanMoveMouse)
	{
		AddControllerPitchInput(value);
	}
}

void AMainPlayer::MouseRightFunc(float value)
{
	if (bCanMoveMouse)
	{
		AddControllerYawInput(value);
	}
}

void AMainPlayer::MoveForward(float value)
{
	if (bCanMove && !bIsDead && !bIsEquip && !bIsUsingSkill && !bIsUsingThirdSkill && cSkillCanMove)
	{
		if (value != 0)
		{
			bIsWalkFront = true;
		}
		else
		{
			bIsWalkFront = false;
		}

		if (value > 0)
		{
			walkForwardValue += value;
		}
		else if (value < 0)
		{
			walkForwardValue -= value;
		}
		
		if (walkForwardValue >= moveSoundValue)
		{
			int randomNum = FMath::RandRange(0, 1);

			if (randomNum == 0)
			{
				UGameplayStatics::PlaySound2D(this, walkSound1);
			}
			else
			{
				UGameplayStatics::PlaySound2D(this, walkSound2);
			}

			walkForwardValue = 0.f;
		}

		YawAxis = GetControlRotation();
		FRotator YawRotMatrixFix = FRotator(0.f, YawAxis.Yaw, 0.f);
		FVector MatrixVector = FRotationMatrix(YawAxis).GetUnitAxis(EAxis::X);

		AddMovementInput(MatrixVector, value);

		cameraLoc = mainCamera->GetComponentLocation();
		orignCameraZ = cameraLoc.Z;
	}
}

void AMainPlayer::MoveRight(float value)
{
	if (bCanMove && !bIsDead && !bIsEquip && !bIsUsingSkill && !bIsUsingThirdSkill && cSkillCanMove)
	{
		if (value > 0)
		{
			walkRightValue += value;
		}
		else if (value < 0)
		{
			walkRightValue -= value;
		}

		if (walkRightValue >= moveSoundValue)
		{
			int randomNum = FMath::RandRange(0, 1);

			if (randomNum == 0 && !bIsWalkFront)
			{
				UGameplayStatics::PlaySound2D(this, walkSound1);
			}
			else if(randomNum == 1 && !bIsWalkFront)
			{
				UGameplayStatics::PlaySound2D(this, walkSound2);
			}

			walkRightValue = 0.f;
		}

		YawAxis = GetControlRotation();
		FRotator YawRotMatrixFix = FRotator(0.f, YawAxis.Yaw, 0.f);
		FVector MatrixVector = FRotationMatrix(YawAxis).GetUnitAxis(EAxis::Y);

		AddMovementInput(MatrixVector, value);
	}
}

void AMainPlayer::Sprint()
{
	bIsSprint = true;

	characterMovement->MaxWalkSpeed = sprintSpeed;
}

void AMainPlayer::EndSprint()
{
	bIsSprint = false;

	characterMovement->MaxWalkSpeed = walkSpeed;
}

void AMainPlayer::InputEquipKey()
{
	if (overlapWeapon != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, weaponGetSound);

		playerAnimInstance->Montage_Play(equipAnimMontage, 1.0f);
		playerAnimInstance->Montage_JumpToSection(FName("EquipWeapon"), attackAnimMontage);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), equipParticle, GetActorLocation() - FVector(0.f, 0.f, 100.f), FRotator(0.f, 90.f, 0.f), true);

		SetEquipStatus(PlayerEquipStatus::PES_EquipWeapon);

		usingWeapon = Cast<AWeapon>(overlapWeapon);
		actorWeapon = Cast<AActor>(overlapWeapon);

		usingWeapon->idleWeaponParticle->Deactivate();

		if (actorWeapon->ActorHasTag("FirstSword"))
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_FirstWeapon);

			weaponSwapNum = 0;

			firstWeapon = actorWeapon;

			if (secondWeapon)
			{
				weaponKeepSocket1->AttachActor(secondWeapon, playerSkeletal);
			}

			if (thirdWeapon)
			{
				weaponKeepSocket2->AttachActor(thirdWeapon, playerSkeletal);
			}

		}
		else if (actorWeapon->ActorHasTag("SecondSword"))
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_SecondWeapon);

			weaponSwapNum = 1;

			secondWeapon = actorWeapon;

			if (firstWeapon)
			{
				weaponKeepSocket1->AttachActor(firstWeapon, playerSkeletal);
			}

			if (thirdWeapon)
			{
				weaponKeepSocket2->AttachActor(thirdWeapon, playerSkeletal);
			}
		}
		else if (actorWeapon->ActorHasTag("ThirdSword"))
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_ThirdWeapon);

			weaponSwapNum = 2;

			thirdWeapon = actorWeapon;

			if (firstWeapon)
			{
				weaponKeepSocket1->AttachActor(firstWeapon, playerSkeletal);
			}

			if (secondWeapon)
			{
				weaponKeepSocket2->AttachActor(secondWeapon, playerSkeletal);
			}
		}
		
		equipSocket->AttachActor(actorWeapon, playerSkeletal);

		overlapWeapon->CollisionOffEquipSphere();
	}
}

void AMainPlayer::WeaponSwap()
{
	UGameplayStatics::PlaySound2D(this, weaponSwapSound);

	if (weaponSwapNum < 3)
	{
		weaponSwapNum++;
	}

	if (thirdWeapon)
	{
		if (weaponSwapNum >= 3)
		{
			weaponSwapNum = 0;
		}
	}
	else
	{
		if (weaponSwapNum >= 2)
		{
			weaponSwapNum = 0;
		}
	}

	switch (weaponSwapNum)
	{
	case 0:
		if (firstWeapon)
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_FirstWeapon);

			actorWeapon = firstWeapon;
			usingWeapon = Cast<AWeapon>(actorWeapon);

			equipSocket->AttachActor(firstWeapon, playerSkeletal);

			playerAnimInstance->Montage_Play(equipAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("EquipWeapon"), attackAnimMontage);

			if (secondWeapon)
			{
				weaponKeepSocket1->AttachActor(secondWeapon, playerSkeletal);
			}

			if (thirdWeapon)
			{
				weaponKeepSocket2->AttachActor(thirdWeapon, playerSkeletal);
			}
		}
		break;
	case 1:
		if (secondWeapon)
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_SecondWeapon);

			actorWeapon = secondWeapon;
			usingWeapon = Cast<AWeapon>(actorWeapon);

			equipSocket->AttachActor(secondWeapon, playerSkeletal);

			playerAnimInstance->Montage_Play(equipAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("EquipWeapon"), attackAnimMontage);

			if (firstWeapon)
			{
				weaponKeepSocket1->AttachActor(firstWeapon, playerSkeletal);
			}

			if (thirdWeapon)
			{
				weaponKeepSocket2->AttachActor(thirdWeapon, playerSkeletal);
			}
		}
		break;
	case 2:
		if (thirdWeapon)
		{
			SetEquipWeapon(PlayerEquipWeapon::PEW_ThirdWeapon);

			actorWeapon = thirdWeapon;
			usingWeapon = Cast<AWeapon>(actorWeapon);

			equipSocket->AttachActor(thirdWeapon, playerSkeletal);

			playerAnimInstance->Montage_Play(equipAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("EquipWeapon"), attackAnimMontage);

			if (firstWeapon)
			{
				weaponKeepSocket1->AttachActor(firstWeapon, playerSkeletal);
			}

			if (secondWeapon)
			{
				weaponKeepSocket2->AttachActor(secondWeapon, playerSkeletal);
			}
		}
		break;
	}
}

void AMainPlayer::Attack()
{
	if (equipStatus == PlayerEquipStatus::PES_EquipWeapon && !bIsDead)
	{
		AutoAimToMonster();

		if (attackStatus == PlayerAttackStatus::PAS_FirstAttack && bCanAttack)
		{
			UGameplayStatics::PlaySound2D(this, usingWeapon->whooshSound1);

			bCanAttack = false;
			bCanMove = false;

			playerAnimInstance->Montage_Play(attackAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("FirstAttack"), attackAnimMontage);

			comboTime = 0.f;

			SetAttackStatus(PlayerAttackStatus::PAS_SecondAttack);
		}
		else if (attackStatus == PlayerAttackStatus::PAS_SecondAttack && bCanAttack)
		{
			UGameplayStatics::PlaySound2D(this, usingWeapon->whooshSound2);

			bCanAttack = false;
			bCanMove = false;

			playerAnimInstance->Montage_Play(attackAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("SecondAttack"), attackAnimMontage);

			comboTime = 0.f;

			SetAttackStatus(PlayerAttackStatus::PAS_ThirdAttack);
		}
		else if (attackStatus == PlayerAttackStatus::PAS_ThirdAttack && bCanAttack)
		{
			UGameplayStatics::PlaySound2D(this, usingWeapon->whooshSound3);

			bCanAttack = false;
			bCanMove = false;
			
			playerAnimInstance->Montage_Play(attackAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("ThirdAttack"), attackAnimMontage);

			comboTime = 0.f;

			SetAttackStatus(PlayerAttackStatus::PAS_ComboAttack);
		}
		else if (attackStatus == PlayerAttackStatus::PAS_ComboAttack && bCanAttack)
		{
			bCanAttack = false;
			bCanMove = false;

			if (equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
			{
				playerAnimInstance->Montage_Play(attackAnimMontage, 1.0f);
				playerAnimInstance->Montage_JumpToSection(FName("FirstWeaponCombo"), attackAnimMontage);
			}
			else if (equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
			{
				playerAnimInstance->Montage_Play(comboAnimMontage, 1.0f);
				playerAnimInstance->Montage_JumpToSection(FName("SecondCombo"), comboAnimMontage);
			}
			else if (equipWeapon == PlayerEquipWeapon::PEW_ThirdWeapon)
			{
				playerAnimInstance->Montage_Play(comboAnimMontage, 1.0f);
				playerAnimInstance->Montage_JumpToSection(FName("ThirdCombo"), comboAnimMontage);
			}

			comboTime = 0.f;

			SetAttackStatus(PlayerAttackStatus::PAS_FirstAttack);
		}
	}
}

void AMainPlayer::CalcComboTime(float deltaTime)
{
	comboTime += deltaTime;

	if (comboTime >= 3.f)
	{
		SetAttackStatus(PlayerAttackStatus::PAS_FirstAttack);
	}
}

void AMainPlayer::HitByEnemy(float enemyDamage)
{
	if (playerHp - enemyDamage <= 0.f)
	{
		playerHp = 0.f;
		bIsDead = true;

		playerCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

		playerAnimInstance->Montage_Play(attackAnimMontage, 1.3f);
		playerAnimInstance->Montage_JumpToSection(FName("Die"), attackAnimMontage);
	}
	else if (playerHp - enemyDamage > 0.f)
	{
		if (bCanAttack && !bIsUsingFirstCSkill && !bIsUsingSkillTime)
		{
			playerAnimInstance->Montage_Play(attackAnimMontage, 1.6f);
			playerAnimInstance->Montage_JumpToSection(FName("Hit"), attackAnimMontage);
		}

		playerHp -= enemyDamage;
	}
}

float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitByEnemy(DamageAmount);

	return DamageAmount;
}

void AMainPlayer::StopDieAnim()
{
	playerSkeletal->bPauseAnims = true;
}

void AMainPlayer::SpawnHitParticle()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, GetActorLocation(), FRotator(0.f), true);
}

void AMainPlayer::ComboSpringArmLocUp(float deltaTime)
{
	SetUsingCombo(PlayerUsingCombo::PUC_UpCombo);

	cameraLoc = mainCamera->GetComponentLocation();

	cameraLoc.Z = FMath::FInterpTo(cameraLoc.Z, cameraLoc.Z + comboHeight, deltaTime, 3.3f);

	mainCamera->SetWorldLocation(cameraLoc);
}

void AMainPlayer::ComboSpringArmLocDown(float deltaTime)
{
	SetUsingCombo(PlayerUsingCombo::PUC_DownCombo);

	cameraLoc = mainCamera->GetComponentLocation();

	cameraLoc.Z = FMath::FInterpTo(cameraLoc.Z, orignCameraZ, deltaTime, 3.3f);

	mainCamera->SetWorldLocation(cameraLoc);
}

void AMainPlayer::SetUpDownComboEnum()
{
	SetUsingCombo(PlayerUsingCombo::PUC_NoCombo);
}

void AMainPlayer::AttackCameraShake()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(cameraShake, 1.0f);
}

void AMainPlayer::ComboSlowMotion()
{
	GetWorldSettings()->SetTimeDilation(0.2f);
}

void AMainPlayer::ComboSlowMotionEnd()
{
	GetWorldSettings()->SetTimeDilation(1.f);
}

void AMainPlayer::PlayerDash(float deltaTime)
{
	dashDistance = FMath::FInterpTo(dashDistance, 50.f, deltaTime, 15.f);
	AddActorWorldOffset(GetActorForwardVector() * dashDistance);

	if (dashDistance >= dashGoalDistance)
	{
		bIsDash = false;

		dashDistance = 0.f;

		dashParticle->Deactivate();
	}
}

void AMainPlayer::SetIsDash()
{
	playerAnimInstance->Montage_Play(attackAnimMontage, 1.0f);
	playerAnimInstance->Montage_JumpToSection(FName("Dash"), attackAnimMontage);

	dashParticle->Activate();

	bIsDash = true;
}

void AMainPlayer::AutoAimToMonster()
{
	if (autoAimMonster != nullptr)
	{
		autoAimRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), autoAimMonster->GetActorLocation());

		FRotator playerRot = GetActorRotation();
		playerRotYaw = playerRot.Yaw;

		SetActorRotation(autoAimRot);
	}
}

void AMainPlayer::RSkill()
{	
	if (!isCoolTimeR && playerMp >= rSkillUseMp)
	{
		playerMp -= rSkillUseMp;
		rSkillCoolTime = rCoolTime;
		isCoolTimeR = true;

		if (equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
		{
			usingWeapon->skillLastHitMonster = nullptr;

			firstRSkillAuraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, firstRSkillAura, GetActorLocation() - FVector(0.f, 0.f, 50.f), FRotator(0.f));

			usingWeapon->firstWeaponNiagara->Activate();

			FRotator cameraRotator = mainCamera->GetComponentRotation();
			FRotator turnPlayerToCameraRot = FRotator(0.f, cameraRotator.Yaw, 0.f);

			SetActorRotation(turnPlayerToCameraRot);

			playerAnimInstance->Montage_Play(attackAnimMontage, 1.0f);
			playerAnimInstance->Montage_JumpToSection(FName("FirstSkill"), attackAnimMontage);
		}
		else if (equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
		{
			bIsChargingSecondR = true;

			FRotator cameraRotator = mainCamera->GetComponentRotation();
			FRotator turnPlayerToCameraRot = FRotator(0.f, cameraRotator.Yaw, 0.f);

			SetActorRotation(turnPlayerToCameraRot);

			playerAnimInstance->Montage_Play(comboAnimMontage, 1.2f);
			playerAnimInstance->Montage_JumpToSection(FName("SecondSkill"), comboAnimMontage);
		}
		else if (equipWeapon == PlayerEquipWeapon::PEW_ThirdWeapon)
		{
			usingWeapon->launchMonsterCount = 0;
			usingWeapon->skillLastHitMonster = nullptr;

			AutoAimToMonster();

			FRotator cameraRotator = mainCamera->GetComponentRotation();
			FRotator turnPlayerToCameraRot = FRotator(0.f, cameraRotator.Yaw, 0.f);

			SetActorRotation(turnPlayerToCameraRot);

			playerAnimInstance->Montage_Play(comboAnimMontage, 1.0f);
			playerAnimInstance->Montage_JumpToSection(FName("ThirdSkill"), comboAnimMontage);
		}
	}
}

void AMainPlayer::DeactivateFirstRNiagara()
{
	usingWeapon->firstWeaponNiagara->Deactivate();
	firstRSkillAuraCompo->Deactivate();
}

void AMainPlayer::SpawnSecondSkillSphere()
{
	UWorld* world = GetWorld();

	FActorSpawnParameters spawnParam;

	spawnSkillActor = world->SpawnActor<ASecondWeaponSkill>(secondSkillSphere, GetActorLocation(), FRotator(0.f), spawnParam);
}

void AMainPlayer::DestroySecondSkillSphere()
{
	spawnSkillActor->Destroy();
}

void AMainPlayer::SkillCameraMove(float deltaTime)
{
	armlength = FMath::FInterpTo(armlength, 200.f, deltaTime, 5.f);

	springArm->TargetArmLength = armlength;
}

void AMainPlayer::SkillCameraMoveEnd(float deltaTime)
{
	armlength = FMath::FInterpTo(armlength, 700.f, deltaTime, 3.f);

	if (armlength >= 695.f)
	{
		armlength = 700.f;

		SetUsingSkill(PlayerUsingSkill::PUC_NoSkill);
	}

	springArm->TargetArmLength = armlength;
}

void AMainPlayer::SkillCameraMove2(float deltaTime)
{
	armlength = FMath::FInterpTo(armlength, 1300.f, deltaTime, 5.f);

	springArm->TargetArmLength = armlength;
}

void AMainPlayer::SkillCameraMove2ChargingR(float deltaTime)
{
	armlength = FMath::FInterpTo(armlength, 400.f, deltaTime, 5.f);

	springArm->TargetArmLength = armlength;
}

void AMainPlayer::SkillCameraMoveEnd2(float deltaTime)
{
	armlength = FMath::FInterpTo(armlength, 700.f, deltaTime, 3.f);

	springArm->TargetArmLength = armlength;
}

void AMainPlayer::SetSkillEnumStart()
{
	if (actorWeapon)
	{
		usingWeapon = Cast<AWeapon>(actorWeapon);
		
		if (usingWeapon)
		{
			originWeaponDamage = usingWeapon->weaponDamage;
			usingWeapon->SetWeaponDamage(firstRSkillDamage);
			
			bIsUsingFirstSkill = true;
		}
	}

	SetUsingSkill(PlayerUsingSkill::PUC_SkillStart);
}

void AMainPlayer::SetSkillEnumEnd()
{
	if (usingWeapon)
	{
		usingWeapon->SetWeaponDamage(originWeaponDamage);

		bIsUsingFirstSkill = false;
	}

	SetUsingSkill(PlayerUsingSkill::PUC_SkillEnd);
}

void AMainPlayer::SetEquipBoolStart()
{
	bIsEquip = true;
}

void AMainPlayer::SetEquipBoolEnd()
{
	bIsEquip = false;
}

void AMainPlayer::PlayCombo1Sound()
{
	UGameplayStatics::PlaySound2D(this, usingWeapon->combo1Sound);
}

void AMainPlayer::PlayCombo1LastSound()
{
	UGameplayStatics::PlaySound2D(this, usingWeapon->combo1LastSound);
}

void AMainPlayer::SetIsUsingSkillTrue()
{
	bIsUsingSkill = true;
}

void AMainPlayer::SetIsUsingSkillFalse()
{
	bIsUsingSkill = false;
}

void AMainPlayer::ThirdSkillPlayerLaunchUp()
{
	bIsUsingThirdSkill = true;

	this->LaunchCharacter(FVector(0.f, 0.f, 500.f), true, true);
}

void AMainPlayer::StopPlayer()
{
	characterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
	characterMovement->StopMovementImmediately();
}

void AMainPlayer::StopCSkillPlayer()
{
	if (usingWeapon->skillLastHitMonster)
	{
		characterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
		characterMovement->StopMovementImmediately();
	}
}

void AMainPlayer::MovePlayer()
{
	bIsUsingThirdSkill = false;
	bCanMove = true;
	bCanAttack = true;

	characterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AMainPlayer::PlaySecondSkillMent()
{
	if (equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
	{
		UGameplayStatics::PlaySound2D(this, secondSkillMent);
	}
	else if (equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
	{
		UGameplayStatics::PlaySound2D(this, firstSkillMent);
	}
}

void AMainPlayer::SetWalkingMode()
{
	this->LaunchCharacter(FVector(0.f, 0.f, -800.f), true, true);
}

void AMainPlayer::UseCSkill()
{
	if (!isCoolTimeC && playerMp >= cSkillUseMp)
	{
		playerMp -= cSkillUseMp;
		isCoolTimeC = true;
		cSkillCoolTime = cCoolTime;

		if (equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
		{
			bCanMoveMouse = false;

			playerAnimInstance->Montage_Play(comboAnimMontage, 1.0f);
			playerAnimInstance->Montage_JumpToSection(FName("FirstCSkill"), comboAnimMontage);
		}
		else if (equipWeapon == PlayerEquipWeapon::PEW_SecondWeapon)
		{
			cSkillCanMove = false;

			secondSword = Cast<ASecondSword>(usingWeapon);

			secondcSkillAuraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, secondCSkillAura, GetActorLocation(), FRotator(0.f));

			secondSword->fireSwordNiagara->Activate();

			UGameplayStatics::PlaySound2D(this, secondFireSound);

			FRotator cameraRotator = mainCamera->GetComponentRotation();
			FRotator turnPlayerToCameraRot = FRotator(0.f, cameraRotator.Yaw, 0.f);

			SetActorRotation(turnPlayerToCameraRot);

			Controller->SetControlRotation(FRotator(-20.f, GetControlRotation().Yaw, GetControlRotation().Roll));

			playerAnimInstance->Montage_Play(comboAnimMontage, 1.5f);
			playerAnimInstance->Montage_JumpToSection(FName("SecondCSkillMotion"), comboAnimMontage);

			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::SetUseSecondCSkill, 0.3f);
		}
		else if (equipWeapon == PlayerEquipWeapon::PEW_ThirdWeapon)
		{
			thirdCSkillAuraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, usingWeapon->thirdWeaponCNiagara, GetActorLocation() - FVector(0.f, 0.f, 50.f), FRotator(0.f));

			UGameplayStatics::PlaySound2D(this, thirdCSkillChargingSound);

			playerAnimInstance->Montage_Play(comboAnimMontage, 1.3f);
			playerAnimInstance->Montage_JumpToSection(FName("ThirdCSkill"), comboAnimMontage);
		}
	}
}

void AMainPlayer::PlayThirdCSkillShootSound()
{
	UGameplayStatics::PlaySound2D(this, thirdCSkillShootingSound);
}

void AMainPlayer::TurnOffThirdCSkillAura()
{
	if (thirdCSkillAuraCompo)
	{
		thirdCSkillAuraCompo->Deactivate();
	}
}

void AMainPlayer::TurnOffSecondCFireSword()
{
	secondSword->fireSwordNiagara->Deactivate();
}

void AMainPlayer::TurnOffSecondCSkillAura()
{
	secondcSkillAuraCompo->Deactivate();
}

void AMainPlayer::ThirdCSkill()
{
	UWorld* world = GetWorld();
	
	FActorSpawnParameters param;

	throwSwordActor = world->SpawnActor<AThrowSword>(throwSword, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50.f), FRotator(0.f), param);

	throwSwordActor->SetMainPlayer(this);

	SwordMoveRange = 0.f;

	throwSwordLoc = throwSwordActor->GetActorLocation();
	playerForwardVector = GetActorForwardVector();

	bIsThrowSword = true;
}

void AMainPlayer::SetbIsThrowSword()
{
	GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::DestroyThrowSword, 2.5f);
}

void AMainPlayer::DestroyThrowSword()
{
	SetbIsUsingSkillTime();

	throwSwordActor->Destroy();

	if (spawnDecal[0])
	{
		for (int i = 0; i < spawnDecal.Num(); i++)
		{
			spawnDecal[i]->Destroy();
		}
	}
}

void AMainPlayer::MoveThrowSword(float deltaTime)
{
	if (throwSwordActor)
	{
		SwordMoveRange = FMath::FInterpTo(SwordMoveRange, targetMoveRange1, deltaTime, 8.f);

		throwSwordActor->SetActorLocation(throwSwordLoc + playerForwardVector * SwordMoveRange);
		
		UWorld* world = GetWorld();
		FActorSpawnParameters params;

		spawnDecal.Add(world->SpawnActor<AThirdCSkillDecal>(throwDecal, throwSwordLoc + playerForwardVector * SwordMoveRange, FRotator(0.f), params));

		if (SwordMoveRange >= 890.f)
		{
			bIsThrowSword = false;
		}

		if (!bIsThrowSword)
		{
			SetbIsThrowSword();
		}
	}
}

void AMainPlayer::CSkillCanMove()
{
	cSkillCanMove = true;
}

void AMainPlayer::SetUsingCSkill()
{
	bIsUsingFirstCSkill = true;
	this->LaunchCharacter(FVector(0.f, 0.f, 700.f), true, true);
}

void AMainPlayer::CSkillMovement()
{
	if (usingWeapon->skillLastHitMonster && equipWeapon == PlayerEquipWeapon::PEW_FirstWeapon)
	{
		firstCSkillMonsterLoc = usingWeapon->skillLastHitMonster->GetActorLocation();

		FVector playerLoc = GetActorLocation();

		usingWeapon->skillLastHitMonster->MonsterHitByPlayer(firstCSkillDamage);

		playerAnimInstance->Montage_Play(comboAnimMontage, 1.5f);
		playerAnimInstance->Montage_JumpToSection(FName("FirstCSkillMotion"), comboAnimMontage);

		switch (cSkillMoveCount)
		{
		case 0:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X - 200.f, firstCSkillMonsterLoc.Y, firstCSkillMonsterLoc.Z));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 1:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X + 200.f, firstCSkillMonsterLoc.Y, firstCSkillMonsterLoc.Z));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 2:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X, firstCSkillMonsterLoc.Y + 200.f, firstCSkillMonsterLoc.Z));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 3:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X, firstCSkillMonsterLoc.Y - 200.f, firstCSkillMonsterLoc.Z));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 4:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X - 200.f, firstCSkillMonsterLoc.Y + 200.f, firstCSkillMonsterLoc.Z -200.f));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 5:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X + 200, firstCSkillMonsterLoc.Y - 250.f, firstCSkillMonsterLoc.Z));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 6:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X - 200, firstCSkillMonsterLoc.Y + 200.f, firstCSkillMonsterLoc.Z + 200.f));
			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount++;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::CSkillMovement, 0.25f);
			break;
		case 7:
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), firstCSkillGhostParticle, GetActorLocation(), FRotator(0.f), true);
			SetActorLocation(FVector(firstCSkillMonsterLoc.X + 130.f, firstCSkillMonsterLoc.Y + 130.f, firstCSkillMonsterLoc.Z + 130.f));

			FRotator test = UKismetMathLibrary::FindLookAtRotation(SkillCameraLocationBox->GetComponentLocation(), this->GetActorLocation());
			Controller->SetControlRotation(test);

			firstCSkillAuraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, firstCSkillAura, GetActorLocation(), FRotator(0.f));

			usingWeapon->HitFunction();
			LookCSkillEnemy();
			cSkillMoveCount = 0;
			usingWeapon->skillLastHitMonster->LaunchCharacter(FVector(0.f, 0.f, 120.f), true, true);
			usingWeapon->skillLastHitMonster->GetCharacterMovement()->GravityScale = 0.2f;
			springArm->TargetArmLength = 350.f;
			origSpringArmLoc = springArm->GetComponentLocation();
			springArm->SetWorldLocation(FVector(springArm->GetComponentLocation().X, springArm->GetComponentLocation().Y + 20.f, springArm->GetComponentLocation().Z));
			UGameplayStatics::PlaySound2D(this, usingWeapon->electricSound);
			usingWeapon->firstWeaponNiagara->Activate();
			ComboSlowMotion();
			forward = GetActorForwardVector();
			currentRot = GetActorRotation();
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::PlayFirstCSkillLastAnim, 0.5f);
			break;
		}
	}
}

void AMainPlayer::SetFirstCSkillLastFunc()
{
	SetActorRotation(FRotator(0.f, currentRot.Yaw, 0.f));

	springArm->TargetArmLength = 700.f;
	springArm->SetWorldLocation(origSpringArmLoc);
	bIsUsingFirstCSkill = false;
	characterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	usingWeapon->skillLastHitMonster->PlayHitBySkillLastAnim();
	usingWeapon->skillLastHitMonster->GetCharacterMovement()->GravityScale = 1.f;
	firstCSkillAuraCompo->Deactivate();
	this->LaunchCharacter(FVector(0.f, 0.f, -500.f), true, true);
	usingWeapon->HitFunction();
	usingWeapon->skillLastHitMonster = nullptr;
	usingWeapon->firstWeaponNiagara->Deactivate();

	bCanMoveMouse = true;
}

void AMainPlayer::PlayFirstCSkillLastAnim()
{
	playerAnimInstance->Montage_Play(comboAnimMontage, 1.0f);
	playerAnimInstance->Montage_JumpToSection(FName("FirstCSkillLast"), comboAnimMontage);
}

void AMainPlayer::LookCSkillEnemy()
{
	FRotator playerRot = GetActorRotation();

	autoAimRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), usingWeapon->skillLastHitMonster->GetActorLocation());

	playerRotYaw = playerRot.Yaw;

	SetActorRotation(autoAimRot);
}

void AMainPlayer::SecondCSkill(float deltaTime)
{
	float cSkillMoveRange;
	float targetMoveRange = 200.f;

	cSkillMoveRange = FMath::FInterpTo(0.f, targetMoveRange, deltaTime, 20.f);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * cSkillMoveRange);
}

void AMainPlayer::SetUseSecondCSkill()
{
	secondCSkillSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsUsingSecondCSkill = false;
}

void AMainPlayer::SetUseSecondCSkillOn()
{
	bIsUseCSkill = false;

	secondCSkillSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bIsUsingSecondCSkill = true;
}

void AMainPlayer::BeginOverlapSecondCSkill(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		cSkillHitMonster = Cast<AMonster>(OtherActor);

		if (cSkillHitMonster)
		{
			bCanMoveMouse = false;
			
			PostProcessGrayOnSetting();

			UGameplayStatics::PlaySound2D(this, secondSword->cSkillHitSound);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), secondSword->secondCSkillHitParticle, cSkillHitMonster->GetActorLocation(), FRotator(0.f), true);
		
			cSkillHitMonster->PlayHitBySkillLastAnim();
			
			springArm->TargetArmLength = -1100.f;
			springArm->SetRelativeLocation(FVector(0.f, -120.f, 0.f));
			mainCamera->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
			mainCamera->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

			ComboSlowMotion();

			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainPlayer::SettingSecondCSkillCamera, 0.5f);
		}
	}
}

void AMainPlayer::PostProcessGrayOnSetting()
{
	mainCamera->PostProcessSettings.ColorSaturation.X = 0.1f;
	mainCamera->PostProcessSettings.ColorSaturation.Y = 0.1f;
	mainCamera->PostProcessSettings.ColorSaturation.Z = 0.1f;
}

void AMainPlayer::PostProcessGrayOffSetting()
{
	mainCamera->PostProcessSettings.ColorSaturation.X = 1.f;
	mainCamera->PostProcessSettings.ColorSaturation.Y = 1.f;
	mainCamera->PostProcessSettings.ColorSaturation.Z = 1.f;
}

void AMainPlayer::SettingSecondCSkillCamera()
{
	ComboSlowMotionEnd();

	PostProcessGrayOffSetting();

	springArm->TargetArmLength = 700.f;
	mainCamera->SetRelativeRotation(FRotator(0.f));
	springArm->SetRelativeLocation(FVector(0.f));

	bCanMoveMouse = true;

	CSkillCanMove();
}

void AMainPlayer::PauseCSkillPose()
{
	if (!cSkillHitMonster)
	{
		CSkillCanMove();

		springArm->TargetArmLength = 700.f;
	}
}

void AMainPlayer::CSkillCameraMove(float deltaTime)
{
	float armLength = springArm->TargetArmLength;

	armLength = FMath::FInterpTo(armLength, cSkillSpringArmLength, deltaTime, 5.f);

	springArm->TargetArmLength = armLength;
}

void AMainPlayer::PlayerStun()
{
	AttackCameraShake();

	bCanMove = false;

	stunParticle->Activate();

	playerAnimInstance->Montage_Play(attackAnimMontage, 1.0f);
	playerAnimInstance->Montage_JumpToSection(FName("Stun"), attackAnimMontage);
}

void AMainPlayer::DeactivateStunParticle()
{
	stunParticle->Deactivate();
}

void AMainPlayer::SetbIsUsingSkillTime()
{
	bIsUsingSkillTime = !bIsUsingSkillTime;
}