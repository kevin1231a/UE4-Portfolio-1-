// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstSword.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AFirstSword::AFirstSword()
{
	Tags.Add("FirstSword");

	skillChargingParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChargingParticle"));
	skillChargingParticle->SetupAttachment(GetRootComponent());
	firstWeaponNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FirstRSkillNiagara"));
	firstWeaponNiagara->SetupAttachment(GetRootComponent());
}

void AFirstSword::BeginPlay()
{
	Super::BeginPlay();

	firstWeaponNiagara->Deactivate();
}

void AFirstSword::TurnOnChargingParticle()
{
	//skillChargingParticle->Activate();
}

void AFirstSword::TurnOffChargingParticle()
{
	//skillChargingParticle->Deactivate();
}