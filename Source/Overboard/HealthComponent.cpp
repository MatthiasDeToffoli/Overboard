// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


void UHealthComponent::Initialize()
{
	_CurrentHealthPoint = _MaxHealthPoint;
}

int UHealthComponent::GetHealth()
{
	return _CurrentHealthPoint;
}

bool UHealthComponent::ApplyDamage(int pDamage)
{
	_CurrentHealthPoint -= - pDamage;
	return _CurrentHealthPoint <= 0;
}

void UHealthComponent::Heal(int pHealPoints)
{
	_CurrentHealthPoint = FMath::Min(_CurrentHealthPoint + pHealPoints, _MaxHealthPoint);
}