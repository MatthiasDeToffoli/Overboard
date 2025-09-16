#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/**
 * Component managing health points
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERBOARD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	* Default constructor
	*/
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
	int GetHealth() const;

	/**
	* public getter of _MaxHealthPoint
	*/
	int GetMaxHealth() const;

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
