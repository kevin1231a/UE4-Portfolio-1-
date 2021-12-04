// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType)
enum class PlayerUsingSkill : uint8
{
	PUC_NoSkill UMETA(DisplayName = "PUC_NoSkill"),
	PUC_SkillStart UMETA(DisplayName = "PUC_SkillStart"),
	PUC_SkillEnd UMETA(DisplayName = "PUC_SkillEnd")
};

UENUM(BlueprintType)
enum class PlayerUsingCombo : uint8
{
	PUC_NoCombo UMETA(DisplayName = "NoCombo"),
	PUC_UpCombo UMETA(DisplayName = "UpCombo"),
	PUC_DownCombo UMETA(DisplayName = "DownCombo")
};

UENUM(BlueprintType)
enum class PlayerEquipWeapon : uint8
{
	PEW_NoWeapon UMETA(DisplayName = "NoWeapon"),
	PEW_FirstWeapon UMETA(DisplayName = "FirstWeapon"),
	PEW_SecondWeapon UMETA(DisplayName = "SecondWeapon"),
	PEW_ThirdWeapon UMETA(DisplayName = "ThirdWeapon")
};

UENUM(BlueprintType)
enum class PlayerEquipStatus : uint8
{
	PES_NoWeapon UMETA(DisplayName = "NoWeapon"),
	PES_EquipWeapon UMETA(DisplayName = "EquipWeapon")
};

UENUM(BlueprintType)
enum class PlayerAttackStatus : uint8
{
	PAS_FirstAttack UMETA(DisplayName = "FirstAttack"),
	PAS_SecondAttack UMETA(DisplayName = "SecondAttack"),
	PAS_ThirdAttack UMETA(DisplayName = "ThirdAttack"),
	PAS_ComboAttack UMETA(DisplayName = "ComboAttack")
};

UCLASS()
class FIRSTPORTFOLIORPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* mainCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* springArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
	const class USkeletalMeshSocket* equipSocket;
	class UCharacterMovementComponent* characterMovement;
	class AController* mainController;
	class USkeletalMeshComponent* playerSkeletal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* overlapWeapon;
	FORCEINLINE void SetOverlapWeapon(AWeapon* weapon) { overlapWeapon = weapon; }
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AActor* actorWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* bpCastWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
	class UAnimMontage* attackAnimMontage;
	class UAnimInstance* playerAnimInstance;
	class UCapsuleComponent* playerCapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitParticle")
	class UParticleSystem* hitParticle;
	class AFirstSword* firstSword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DashParticle")
	class UParticleSystemComponent* dashParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AutoAimMonster")
	class AMonster* autoAimMonster;
	FORCEINLINE void SetAutoAimMonster(AMonster* monster) { autoAimMonster = monster; }
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillCameraLocationBox")
	class UBoxComponent* SkillCameraLocationBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillCameraLocationBox")
	class UBoxComponent* SkillCameraOriginLocationBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UsingWeapon")
	class AWeapon* usingWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
	class UAnimMontage* equipAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboAnimMontage")
	class UAnimMontage* comboAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipParticle")
	class UParticleSystem* equipParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "swapParticle")
	class UParticleSystem* swapParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FirstWeapon")
	class AActor* firstWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondWeapon")
	class AActor* secondWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThirdWeapon")
	class AActor* thirdWeapon;
	const class USkeletalMeshSocket* weaponKeepSocket1;
	const class USkeletalMeshSocket* weaponKeepSocket2;
	class ASecondWeaponSkill* spawnSkillActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondSkillMent")
	class USoundCue* secondSkillMent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FirstSkillMent")
	class USoundCue* firstSkillMent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GhostParticle")
	class UParticleSystem* firstCSkillGhostParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondCSkillSphere")
	class USphereComponent* secondCSkillSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondCSkillSphere")
	class AMonster* cSkillHitMonster;
	class ASecondSword* secondSword;
	class AThrowSword* throwSwordActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WalkSound")
	class USoundCue* walkSound1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WalkSound")
	class USoundCue* walkSound2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FirstRSkillAura")
	class UNiagaraSystem* firstRSkillAura;
	class UNiagaraComponent* firstRSkillAuraCompo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponGetSound")
	class USoundCue* weaponGetSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSwapSound")
	class USoundCue* weaponSwapSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponFireSound")
	class USoundCue* secondFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FirstCSkillAura")
	class UNiagaraSystem* firstCSkillAura;
	class UNiagaraComponent* firstCSkillAuraCompo;
	class UNiagaraComponent* thirdCSkillAuraCompo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondCSkillAura")
	class UNiagaraSystem* secondCSkillAura;
	class UNiagaraComponent* secondcSkillAuraCompo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThirdCSkillSound")
	class USoundCue* thirdCSkillChargingSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThirdCSkillSound")
	class USoundCue* thirdCSkillShootingSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StunParticle")
	class UParticleSystemComponent* stunParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipStatus")
	PlayerEquipStatus equipStatus;
	FORCEINLINE void SetEquipStatus(PlayerEquipStatus status) { equipStatus = status; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackStatus")
	PlayerAttackStatus attackStatus;
	FORCEINLINE void SetAttackStatus(PlayerAttackStatus status) { attackStatus = status; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipWeapon")
	PlayerEquipWeapon equipWeapon;
	FORCEINLINE void SetEquipWeapon(PlayerEquipWeapon weapon) { equipWeapon = weapon; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usingcombo")
	PlayerUsingCombo usingCombo;
	FORCEINLINE void SetUsingCombo(PlayerUsingCombo combo) { usingCombo = combo; }
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UsingSkill")
	PlayerUsingSkill usingSkill;
	FORCEINLINE void SetUsingSkill(PlayerUsingSkill skill) { usingSkill = skill; }

	FORCEINLINE void CalcDamage(float damage) { playerHp -= damage; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool bIsSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanAttack")
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CanMove")
	bool bCanMove;
	bool bIsDead;
	FRotator YawAxis;
	FVector YawRotMatrix;
	float orignCameraZ;
	FVector orignCameraVector;
	float walkSpeed;
	float sprintSpeed;
	float comboTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerHp")
	float playerHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerMp")
	float playerMp;
	float comboHeight;
	FVector cameraLoc;
	float dashDistance;
	float dashGoalDistance;
	bool bIsDash;
	FRotator autoAimRot;
	float playerRotYaw;
	float armlength = 550.f;
	float firstRSkillDamage;
	float originWeaponDamage;
	float firstCSkillDamage;
	FVector firstCSkillMonsterLoc;
	bool bIsEquip;
	bool bIsUsingSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsUsingFirstCSkill")
	bool bIsUsingFirstCSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IsUsingThirdSkill")
	bool bIsUsingThirdSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IsUsingFirstSkill")
	bool bIsUsingFirstSkill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SwapNum")
	int weaponSwapNum;
	int cSkillMoveCount;
	FTimerHandle timerHandle;
	bool bIsUsingSecondCSkill;
	bool bCanMoveMouse;
	FVector firstCameraLoc;
	FRotator firstCameraRot;
	float cSkillSpringArmLength;
	bool bIsUseCSkill;
	bool cSkillCanMove;
	FVector throwSwordLoc;
	bool bIsThrowSword;
	FVector playerForwardVector;
	FVector origSpringArmLoc;
	float SwordMoveRange;
	float targetMoveRange1;
	float walkForwardValue;
	float walkRightValue;
	bool bIsWalkFront;
	bool bFirstPlayWalkSound;
	bool bFirstPlayWalkSound1;
	bool bFirstCSkill;
	float moveSoundValue;
	FVector forward;
	FRotator currentRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsChargingSecondR")
	bool bIsChargingSecondR;
	bool bIsUsingSkillTime;
	const float rCoolTime = 8.f;
	const float cCoolTime = 11.f;
	const float rSkillUseMp = 10.f;
	const float cSkillUseMp = 20.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RSkillCoolTime");
	float rSkillCoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CSkillCoolTime");
	float cSkillCoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RSkillCool");
	bool isCoolTimeR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CSkillCool");
	bool isCoolTimeC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shake")
	TSubclassOf<UMatineeCameraShake> cameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondWeaponSkill")
	TSubclassOf<class ASecondWeaponSkill> secondSkillSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThrowSword")
	TSubclassOf<class AThrowSword> throwSword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThrowSwordDecal")
	TSubclassOf<class AThirdCSkillDecal> throwDecal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThrowSwordDecal")
	TArray<AThirdCSkillDecal*> spawnDecal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Sprint();
	void EndSprint();
	void InputEquipKey();
	void RSkill();
	void Attack();
	void Dodge();
	void WeaponSwap();
	void CalcComboTime(float deltaTime);
	void HitByEnemy(float enemyDamage);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	void StopDieAnim();
	void SpawnHitParticle();
	UFUNCTION(BlueprintCallable)
	void ComboSpringArmLocUp(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void ComboSpringArmLocDown(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void SetUpDownComboEnum();
	void AttackCameraShake();
	UFUNCTION(BlueprintCallable)
	void ComboSlowMotion();
	UFUNCTION(BlueprintCallable)
	void ComboSlowMotionEnd();
	void PlayerDash(float deltaTime);
	void SetIsDash();
	void AutoAimToMonster();
	void SkillCameraMove(float deltaTime);
	void SkillCameraMoveEnd(float deltaTime);
	void SkillCameraMove2(float deltaTime);
	void SkillCameraMoveEnd2(float deltaTime);
	void SkillCameraMove2ChargingR(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void SetSkillEnumStart();
	UFUNCTION(BlueprintCallable)
	void SetSkillEnumEnd();
	UFUNCTION(BlueprintCallable)
	void SetEquipBoolStart();
	UFUNCTION(BlueprintCallable)
	void SetEquipBoolEnd();
	UFUNCTION(BlueprintCallable)
	void PlayCombo1Sound();
	UFUNCTION(BlueprintCallable)
	void PlayCombo1LastSound();
	UFUNCTION(BlueprintCallable)
	void SpawnSecondSkillSphere();
	UFUNCTION(BlueprintCallable)
	void DestroySecondSkillSphere();
	UFUNCTION(BlueprintCallable)
	void SetIsUsingSkillTrue();
	UFUNCTION(BlueprintCallable)
	void SetIsUsingSkillFalse();
	UFUNCTION(BlueprintCallable)
	void ThirdSkillPlayerLaunchUp();
	UFUNCTION(BlueprintCallable)
	void StopPlayer();
	UFUNCTION(BlueprintCallable)
	void MovePlayer();
	UFUNCTION(BlueprintCallable)
	void PlaySecondSkillMent();
	UFUNCTION(BlueprintCallable)
	void SetWalkingMode();
	void UseCSkill();
	UFUNCTION(BlueprintCallable)
	void SetUsingCSkill();
	UFUNCTION(BlueprintCallable)
	void CSkillMovement();
	void LookCSkillEnemy();
	UFUNCTION(BlueprintCallable)
	void StopCSkillPlayer();
	void SecondCSkill(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void SetUseSecondCSkill();
	UFUNCTION(BlueprintCallable)
	void SetUseSecondCSkillOn();
	UFUNCTION()
	void BeginOverlapSecondCSkill(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SettingSecondCSkillCamera();
	void MouseUpFunc(float value);
	void MouseRightFunc(float value);
	UFUNCTION(BlueprintCallable)
	void PauseCSkillPose();
	void CSkillCameraMove(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void CSkillCanMove();
	UFUNCTION(BlueprintCallable)
	void ThirdCSkill();
	void MoveThrowSword(float deltaTime);
	void SetbIsThrowSword();
	void DestroyThrowSword();
	UFUNCTION(BlueprintCallable)
	void DeactivateFirstRNiagara();
	void PlayFirstCSkillLastAnim();
	UFUNCTION(BlueprintCallable)
	void SetFirstCSkillLastFunc();
	void PostProcessGrayOnSetting();
	void PostProcessGrayOffSetting();
	UFUNCTION(BlueprintCallable)
	void TurnOffSecondCSkillAura();
	UFUNCTION(BlueprintCallable)
	void TurnOffSecondCFireSword();
	UFUNCTION(BlueprintCallable)
	void TurnOffThirdCSkillAura();
	UFUNCTION(BlueprintCallable)
	void PlayThirdCSkillShootSound();
	void PlayerStun();
	UFUNCTION(BlueprintCallable)
	void DeactivateStunParticle();
	UFUNCTION(BlueprintCallable)
	void SetbIsUsingSkillTime();

	void CalcRSkillCoolTime(float DeltaTime);
	void CalcCSkillCoolTime(float DeltaTime);
};