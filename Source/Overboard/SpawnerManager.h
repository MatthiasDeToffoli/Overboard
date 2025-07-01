// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

UCLASS()
class OVERBOARD_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
private:
	/**
	* spawner of the enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	TArray<AActor*> _spawners;

	/**
	* minimum time to spawn enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float _minSpawnTime = 1;

	/**
	* maximum time to spawn enemies
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float _maxSpawnTime = 5;

	/**
	* Time to spawn enemies
	*/
	float _currentSpawnTime = 0;

	/**
	* time between two spawns
	*/
	float _currentTimeBetweenSpawns = 0;

	/**
	* Factor remove from the current spawn time to make it accelerate
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	float _spawnTimeDiffFactor = 0.1f;

	/**
	* enemies class to spawn
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy spawn")
	TSubclassOf<ABaseTargetable> _enemyClass;

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
