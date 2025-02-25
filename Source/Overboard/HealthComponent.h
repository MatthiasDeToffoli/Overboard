// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERBOARD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

private:
	/**
	 * Current health point value
	*/
	int _CurrentHealthPoint;

	/**
	 * Max health point value
	 */
	UPROPERTY(EditAnywhere)
	int _MaxHealthPoint;

protected:
	/**
	 * Called when the game starts
	*/
	virtual void BeginPlay() override;

public:	
	/**
	 * Initialize the health point
	*/
	void Initialize();

	/**
	 * public getter of _CurrentHealthPoint 
	*/
	int GetHealth();

	/**
	 * Apply damage to the healthpoints
	 * 
	 * @param pDamage damage to apply
	*/
	bool ApplyDamage(int pDamage);

	/**
	 * Add new health point to the current one
	 * 
	 * @param pHealPoints the healing value
	*/
	void Heal(int pHealPoints);

		
};
