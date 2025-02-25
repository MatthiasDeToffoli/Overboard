// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "BaseTargetable.generated.h"

UCLASS()
class OVERBOARD_API ABaseTargetable : public AActor
{
	GENERATED_BODY()

private:
	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _mainContainer;

	/**
	* Widget of the target will show to the player which is targeted
	*/
	UPROPERTY(EditAnywhere, Category = "Target")
	UWidgetComponent* _targetWidgetComponent;

	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graphism", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _graphismContainer;

	/**
	*Main mesh of the Targetable actor
	*/
	UPROPERTY(EditAnywhere, Category = "Graphism")
	UStaticMeshComponent* _mainMesh;

	//Health -----------------------------------------------------------------------------------------------------
	/**
	* Health of the player
	*/
	UPROPERTY(EditAnywhere, Category = "Health")
	UHealthComponent* _healthComponent;
	
public:	
	// Sets default values for this actor's properties
	ABaseTargetable();

protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public :
	/**
	*Call when the actor is targeted by the player
	* 
	* @param pIsTargeted if the actor is targeted by the player or not
	*/
	void SetTargeted(bool pIsTargeted);

	/**
	* Update the Target rotation to always look at the player
	* 
	* @param pPlayerPos player position
	* @param pDeltaTime tick's delta time
	*/
	void UpdateTargetRotation(FVector pPlayerPos, float pDeltaTime);

	/**
	* Functioàn to handle demages
	* 
	* @param DamageAmount amount of damage to apply
	* @param DamageEvent event of the damage
	* @param EventInstigator instigator of the damage
	* @param DamageCauser causer of the damage
	*/
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
