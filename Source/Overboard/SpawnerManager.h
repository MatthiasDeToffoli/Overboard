#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

class ABaseTargetable;

/**
* Manage the random spawn of the enemies
*/
UCLASS()
class OVERBOARD_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
private:
	/**
	* spawner of the enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	TArray<AActor*> spawners_;

	/**
	* minimum time to spawn enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float minSpawnTime_ = 1;

	/**
	* maximum time to spawn enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float maxSpawnTime_ = 5;

	/**
	* Time to spawn enemies
	*/
	float currentSpawnTime_ = 0;

	/**
	* time between two spawns
	*/
	float currentTimeBetweenSpawns_ = 0;

	/**
	* Factor remove from the current spawn time to make it accelerate
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float spawnTimeDiffFactor_ = 0.1f;

	/**
	* enemies class to spawn
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	TSubclassOf<ABaseTargetable> enemyClass_;

public:
	/**
	* if spawning is enabled
	*/
	bool IsSpawningEnabled = false;

public:	
	// Sets default values for this actor's properties
	ASpawnerManager();

private:
	/**
	* Spawn an enemy at a random spawner
	*/
	void SpawnEnemy();

protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * Called every frame
	 *
	 * @param deltatime between two ticks
	 */
	virtual void Tick(float pDeltaTime) override;

};
