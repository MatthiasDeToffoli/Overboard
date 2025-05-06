// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
    NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player) return EBTNodeResult::Failed;

    FVector PlayerLocation = Player->GetActorLocation();

    // Met à jour le Blackboard
    OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", PlayerLocation);

    return EBTNodeResult::Succeeded;
}