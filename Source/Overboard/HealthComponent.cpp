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
	currentHealthPoint_ = maxHealthPoint_;
}

int UHealthComponent::GetHealth() const
{
	return currentHealthPoint_;
}

int UHealthComponent::GetMaxHealth() const
{
	return maxHealthPoint_;
}

bool UHealthComponent::ApplyDamage(int pDamage)
{
	currentHealthPoint_ -= pDamage;
	return currentHealthPoint_ <= 0;
}

void UHealthComponent::Heal(int pHealPoints)
{
	
	currentHealthPoint_ = FMath::Min(currentHealthPoint_ + pHealPoints, maxHealthPoint_);
}