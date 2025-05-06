// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFlyingMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTTaskFlyingMoveTo::UBTTaskFlyingMoveTo()
{
    bNotifyTick = true;
    NodeName = "Fly To Target";
}

uint16 UBTTaskFlyingMoveTo::GetInstanceMemorySize() const
{
    return sizeof(FVector);
}

ABaseEnemy* UBTTaskFlyingMoveTo::GetEnemyPawn(UBehaviorTreeComponent& pOwnerComp) const
{
	AAIController* lController = pOwnerComp.GetAIOwner();

	if (lController)
	{
		return Cast<ABaseEnemy>(lController->GetPawn());
	}

	return nullptr;
}

bool UBTTaskFlyingMoveTo::CheckHasObstacle(FVector pCurrentLocation, FVector pDirection, float pAvoidDistance)
{
	FHitResult lHit;
    return GetWorld()->LineTraceSingleByChannel(
		lHit,
		pCurrentLocation,
		pCurrentLocation + pDirection * pAvoidDistance,
		ECC_WorldStatic
	);
}

EBTNodeResult::Type UBTTaskFlyingMoveTo::ExecuteTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory)
{
    ABaseEnemy* lEnemy = GetEnemyPawn(pOwnerComp);

    if (lEnemy) 
    {
        UBlackboardComponent* lBB = pOwnerComp.GetBlackboardComponent();
        targetLocation = lBB->GetValueAsVector("TargetLocation");  // Your blackboard key

        return EBTNodeResult::InProgress;
    }
        
    return EBTNodeResult::Failed;
}

void UBTTaskFlyingMoveTo::TickTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory, float pDeltaSeconds)
{
    ABaseEnemy* lEnemy = GetEnemyPawn(pOwnerComp);

    if (lEnemy) 
    {
        FVector lCurrentLocation = lEnemy->GetActorLocation();

        FVector lFinalDirection = (targetLocation - lCurrentLocation).GetSafeNormal();

        FVector Forward = lEnemy->GetActorForwardVector();
        FVector Right = lEnemy->GetActorRightVector();

        float AvoidDistance = lEnemy->wallAvoidingDistance;

        if (CheckHasObstacle(lCurrentLocation, Forward, AvoidDistance))
        {
            if (lEnemy->isStupid) 
            {
                return;
            }

            if (!CheckHasObstacle(lCurrentLocation, Forward + Right, AvoidDistance))
            {
                lFinalDirection += Right;
            }
            else if (!CheckHasObstacle(lCurrentLocation, Forward - Right, AvoidDistance))
            {
                lFinalDirection -= Right;
            }
            else
            {
                lFinalDirection += FVector::UpVector;
            }
                
        }

        FVector lNewLocation = lCurrentLocation + lFinalDirection.GetSafeNormal() * lEnemy->speed * pDeltaSeconds;
        lEnemy->SetActorLocation(lNewLocation);

        if (FVector::Dist(targetLocation, lNewLocation) <= 0.0f)
        {
            FinishLatentTask(pOwnerComp, EBTNodeResult::Succeeded);
        }
    }

    FinishLatentTask(pOwnerComp, EBTNodeResult::Failed);
    return;
}