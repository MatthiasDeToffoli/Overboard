#include "HealthComponent.h"
#include "Math/UnrealMathUtility.h"

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

int UHealthComponent::GetHealth() const
{
	return _CurrentHealthPoint;
}

int UHealthComponent::GetMaxHealth() const
{
	return _MaxHealthPoint;
}

bool UHealthComponent::ApplyDamage(int pDamage)
{
	_CurrentHealthPoint -= pDamage;
	return _CurrentHealthPoint <= 0;
}

void UHealthComponent::Heal(int pHealPoints)
{
	
	_CurrentHealthPoint = FMath::Min(_CurrentHealthPoint + pHealPoints, _MaxHealthPoint);
}