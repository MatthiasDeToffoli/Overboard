#include "BTTaskFlyingMoveTo.h"
#include "AIController.h"
#include "BaseEnemy.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Constants.h"
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

bool UBTTaskFlyingMoveTo::CheckHasObstacle(FVector pCurrentLocation, FVector pDirection, float pAvoidDistance, FCollisionQueryParams pParams)
{
    FHitResult lHit;
    return GetWorld()->LineTraceSingleByChannel(
        lHit,
        pCurrentLocation,
        pCurrentLocation + pDirection * pAvoidDistance,
        ECollisionChannel::ECC_WorldStatic,
        pParams
    );
}

bool UBTTaskFlyingMoveTo::CheckHasObstacle(FVector pCurrentLocation, FVector pDirection, float pAvoidDistance, float offset, FCollisionQueryParams pParams)
{
	bool lResult = false;
    TArray<FVector> lPositionsToCheck =
    {
        pCurrentLocation,
        pCurrentLocation + FVector(offset, 0.f, 0.f),
        pCurrentLocation + FVector(offset, offset, 0.f),
        pCurrentLocation - FVector(offset, offset, 0.f),
    };

	for (const FVector& lPos : lPositionsToCheck)
	{
		lResult = CheckHasObstacle(lPos, pDirection, pAvoidDistance, pParams);
		
        if (lResult)
		{
			break;
		}
	}
	
    return lResult;
}

EBTNodeResult::Type UBTTaskFlyingMoveTo::ExecuteTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory)
{
    ABaseEnemy* lEnemy = GetEnemyPawn(pOwnerComp);

    if (lEnemy) 
    {
        UBlackboardComponent* lBB = pOwnerComp.GetBlackboardComponent();
        targetLocation_ = lBB->GetValueAsVector(Constants::BlackBoard::kTargetLocation());  // Your blackboard key

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
        FCollisionQueryParams lParams;
		lParams.AddIgnoredActor(lEnemy);
        FVector lFinalDirection = (targetLocation_ - lCurrentLocation).GetSafeNormal();

        FVector lRight = lEnemy->GetActorRightVector();

        float lAvoidDistance = lEnemy->wallAvoidingDistance;
        float lOffset = lEnemy->avoidingOffsetValue;

        if (CheckHasObstacle(lCurrentLocation, lEnemy->GetActorForwardVector(), lAvoidDistance, lOffset , lParams))
        {
            if (lEnemy->isStupid) 
            {
                return;
            }

            if (!CheckHasObstacle(lCurrentLocation, lRight, lAvoidDistance, lParams))
            {
                lFinalDirection = lRight;
            }
            else if (!CheckHasObstacle(lCurrentLocation, -lRight, lAvoidDistance, lParams))
            {
                lFinalDirection = -lRight;
            }
            else
            {
                lFinalDirection += FVector::UpVector;
            }
                
        }

        FVector lNewLocation = lCurrentLocation + lFinalDirection.GetSafeNormal() * lEnemy->speed * pDeltaSeconds;
        lEnemy->SetActorLocation(lNewLocation);

        if (FVector::Dist(targetLocation_, lNewLocation) <= 0.0f)
        {
            FinishLatentTask(pOwnerComp, EBTNodeResult::Succeeded);
        }
    }

    FinishLatentTask(pOwnerComp, EBTNodeResult::Failed);
    return;
}