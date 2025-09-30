#pragma once

#include "CoreMinimal.h"
#include "BaseTargetable.generated.h"

class UWidgetComponent;
class UHealthComponent;

/**
 * Base class for all actors which can be targeted by the player
 */
UCLASS()
class OVERBOARD_API ABaseTargetable : public APawn
{
	GENERATED_BODY()

private:
	/**
	* Widget of the target will show to the player which is targeted
	*/
	UPROPERTY(EditAnywhere, Category = "Target")
	UWidgetComponent* targetWidgetComponent_;

	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graphism", meta = (AllowPrivateAccess = "true"))
	USceneComponent* graphismContainer_;

	/**
	*Main mesh of the Targetable actor
	*/
	UPROPERTY(EditAnywhere, Category = "Graphism")
	UStaticMeshComponent* mainMesh_;

	//Health -----------------------------------------------------------------------------------------------------
	/**
	* Health of the player
	*/
	UPROPERTY(EditAnywhere, Category = "Health")
	UHealthComponent* healthComponent_;
	
	//Score -----------------------------------------------------------------------------------------------------
	/**
	* Score to give to the player when the target is destroyed
	*/
	UPROPERTY(EditAnywhere, Category = "Score")
	int scoreToGive_ = 10;
protected :
	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = "true"))
	USceneComponent* mainContainer_;
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
	virtual float TakeDamage(float pDamageAmount, FDamageEvent const& pDamageEvent, AController* pEventInstigator, AActor* pDamageCauser) override;

};
