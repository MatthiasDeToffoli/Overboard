// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManager.h"

// Sets default values
ASpawnerManager::ASpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_spawners = TArray<AActor*>();
}

// Called when the game starts or when spawned
void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
    IsSpawningEnabled = false;
	// Initialize spawn time
	_currentSpawnTime = _maxSpawnTime;
	_currentTimeBetweenSpawns = 0;
}

// Called every frame
void ASpawnerManager::Tick(float pDeltaTime)
{
	Super::Tick(pDeltaTime);

    if (IsSpawningEnabled)
    {
        _currentTimeBetweenSpawns += pDeltaTime;

        if (_currentTimeBetweenSpawns >= _currentSpawnTime)
            SpawnEnemy();
    }
	
}

void ASpawnerManager::SpawnEnemy()
{
    if (_spawners.Num() == 0)
    {
        return;
    }
    else
    {
        // Select a random spawner
        int32 lRandomIndex = FMath::RandRange(0, _spawners.Num() - 1);
        AActor* lSpawner = _spawners[lRandomIndex];
        if (lSpawner)
        {
            // Spawn the enemy at the selected spawner's location
            FVector lSpawnLocation = lSpawner->GetActorLocation();
            FRotator lSpawnRotation = lSpawner->GetActorRotation();
            ABaseTargetable* lEnemy = GetWorld()->SpawnActor<ABaseTargetable>(_enemyClass, lSpawnLocation, lSpawnRotation);
            if (lEnemy)
            {
                //Get the new spawn time taking care to not go under the minimum
                _currentSpawnTime = FMath::Max(_minSpawnTime, _currentSpawnTime - _spawnTimeDiffFactor);

                // Reset spawn time, favorising the player
                _currentTimeBetweenSpawns = 0;
            }
        }

    }
}
