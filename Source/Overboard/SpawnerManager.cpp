#include "SpawnerManager.h"
#include "BaseTargetable.h"

// Sets default values
ASpawnerManager::ASpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawners_ = TArray<AActor*>();
}

// Called when the game starts or when spawned
void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
    IsSpawningEnabled = false;
	// Initialize spawn time
	currentSpawnTime_ = maxSpawnTime_;
	currentTimeBetweenSpawns_ = 0;
}

// Called every frame
void ASpawnerManager::Tick(float pDeltaTime)
{
	Super::Tick(pDeltaTime);

    if (IsSpawningEnabled)
    {
        currentTimeBetweenSpawns_ += pDeltaTime;

        if (currentTimeBetweenSpawns_ >= currentSpawnTime_)
            SpawnEnemy();
    }
	
}

void ASpawnerManager::SpawnEnemy()
{
    if (spawners_.Num() == 0)
    {
        return;
    }
    else
    {
        int32 lRandomIndex = FMath::RandRange(0, spawners_.Num() - 1);
        AActor* lSpawner = spawners_[lRandomIndex];
        if (lSpawner)
        {
            FVector lSpawnLocation = lSpawner->GetActorLocation();
            FRotator lSpawnRotation = lSpawner->GetActorRotation();
            ABaseTargetable* lEnemy = GetWorld()->SpawnActor<ABaseTargetable>(enemyClass_, lSpawnLocation, lSpawnRotation);
            if (lEnemy)
            {
                currentSpawnTime_ = FMath::Max(minSpawnTime_, currentSpawnTime_ - spawnTimeDiffFactor_);
                currentTimeBetweenSpawns_ = 0;
            }
        }

    }
}
