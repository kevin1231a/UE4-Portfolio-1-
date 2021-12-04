// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondSword.h"
#include "NiagaraComponent.h"

ASecondSword::ASecondSword()
{
	Tags.Add("SecondSword");

	fireSwordNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SwordNiagara"));
	fireSwordNiagara->SetupAttachment(GetRootComponent());
}