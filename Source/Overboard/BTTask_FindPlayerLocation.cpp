#include "BTTask_FindPlayerLocation.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Constants.h"
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
    NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACharacter* lPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (lPlayer)
    {
        FVector PlayerLocation = lPlayer->GetActorLocation();
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(Constants::BlackBoard::kTargetLocation(), PlayerLocation);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}