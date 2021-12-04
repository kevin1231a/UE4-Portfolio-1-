// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class MonsterStatus : uint8
{
	MS_Idle UMETA(DisplayName = "Idle"),
	MS_Chase UMETA(DisplayName = "Chase"),
	MS_Attack UMETA(DisplayName = "Attack"),
	MS_Die UMETA(DisplayName = "Die")
};

UCLASS()
class FIRSTPORTFOLIORPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCapsule")
	class UCapsuleComponent* hitCapsule;
	class AAIController* aiController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChaseSphere")
	class USphereComponent* chaseSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetPlayer")
	class AMainPlayer* targetPlayer;
	class UCharacterMovementComponent* monsterMovementComponent;
	class USkeletalMeshComponent* enemySkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSkeletal")
	class USkeletalMeshComponent* enemyWeaponSkeletal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackBox")
	class UBoxComponent* enemyAttackBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAnimMontage")
	class UAnimMontage* enemyAnimMontage;
	class UAnimInstance* enemyAnimInstance;
	FTimerHandle timerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterDieParticle")
	class UParticleSystem* monsterDieParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackRangeSphere")
	class USphereComponent* attackRangeSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TargetPlayer")
	class AMainPlayer* chaseTargetPlayer;
	FORCEINLINE void SetChaseTargetPlayer(AMainPlayer* player) { chaseTargetPlayer = player; }
	class AActor* chaseTargetActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossMonster")
	class ABossMonster* bossMonster;
	FORCEINLINE void SetBossMonster(ABossMonster* boss) { bossMonster = boss; }
	class UGameInstance* gi;
	class UGI* myGI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubclassMainPlayer")
	TSubclassOf<class AMainPlayer> subclassMainPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OnGroundParticle")
	class UParticleSystem* onGroundParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HPUMG")
	class UWidgetComponent* hpUMG;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterStatus")
	MonsterStatus monsterStatusEnum;
	FORCEINLINE void SetMonsterStatus(MonsterStatus status) { monsterStatusEnum = status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterHp")
	float monsterHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterDamage")
	float monsterDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterDieParticle")
	FVector dieParticleLoc;
	bool bIsInAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsAttack")
	bool bIsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsAttack")
	bool bCanChase;
	bool bCanAttack;
	bool bIsFallingByHit;
	bool bIsUseSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsTurnOnHpBar")
	bool bIsTurnOnHpBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageType")
	TSubclassOf<UDamageType> damageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginOverlapChaseSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapChaseSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void MonsterDie();
	void MonsterHitByPlayer(float weaponDamage);
	void MonsterDestroyTimer();
	UFUNCTION()
	void BeginOverlapAttackRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	virtual void MonsterAttackLoop();
	UFUNCTION()
	void EndOverlapAttackRangeSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void ChaseTarget();
	virtual void HitAnimation();
	UFUNCTION(BlueprintCallable)
	void AttackBoxCollisionOn();
	UFUNCTION(BlueprintCallable)
	void AttackBoxCollisionOff();
	void SecondSkillLaunch();
	void SecondSkillLastLaunch();
	UFUNCTION(BlueprintCallable)
	void PlayHitBySkillLastAnim();
	UFUNCTION(BlueprintCallable)
	void SetCanChase();
	UFUNCTION(BlueprintCallable)
	void SetCanAttack();
	UFUNCTION(BlueprintCallable)
	void CheckIsOnGround();
	void PlayViewEmotion();
};