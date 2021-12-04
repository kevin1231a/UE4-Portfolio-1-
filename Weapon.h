// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	WT_FirstWeapon UMETA(DisplayName = "FirstWeapon"),
	WT_SecondWeapon UMETA(DisplayName = "SecondWeapon"),
	WT_ThirdWeapon UMETA(DisplayName = "ThirdWeapon")
};

UCLASS()
class FIRSTPORTFOLIORPG_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSkeletal")
	class USkeletalMeshComponent* weaponSkeletal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponEquipSphere")
	class USphereComponent* weaponEquipSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponUsePlayer")
	class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponIdleParticle")
	class UParticleSystemComponent* weaponComboParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponAttackBox")
	class UBoxComponent* attackBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackMonster")
	class AMonster* attackMonster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponHitParticle")
	class UParticleSystem* weaponHitParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackMonster")
	const class USkeletalMeshSocket* weaponHitParticleSocket;
	class AMainPlayer* mainPlayerToCameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponWhooshSound1")
	class USoundCue* whooshSound1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponWhooshSound1")
	class USoundCue* whooshSound2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponWhooshSound1")
	class USoundCue* whooshSound3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponWhooshSound1")
	class USoundCue* combo1Sound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponWhooshSound1")
	class USoundCue* combo1LastSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponParticle")
	class UParticleSystemComponent* idleWeaponParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillHitMonster")
	class AMonster* skillLastHitMonster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charg")
	class UNiagaraComponent* firstWeaponNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ElectricSoundCue")
	class USoundCue* electricSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ElectricSoundCue")
	class UNiagaraSystem* firstWeaponHitNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThirdWeaponNiagara")
	class UNiagaraSystem* thirdWeaponCNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitSound")
	TArray<USoundCue*> hitSoundArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDamage")
	float weaponDamage;
	FORCEINLINE void SetWeaponDamage(float skillDamage) { weaponDamage = skillDamage; }
	WeaponType weaponTypeEnum;
	FORCEINLINE void SetWeaponType(WeaponType weapon) { weaponTypeEnum = weapon; }
	
	int randomPlayNum;
	int launchMonsterCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlapEquipSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapEquipSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CollisionOffEquipSphere();
	UFUNCTION(BlueprintCallable)
	void CollisionOffAttackBox();
	UFUNCTION(BlueprintCallable)
	void CollisionOnAttackBox();
	void HitFunction();
	UFUNCTION(BlueprintCallable)
	void DeActivateFirstNiagara();
};
