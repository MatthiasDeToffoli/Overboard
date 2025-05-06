// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ScreenLogger.h"

AFlyingEnemyController::AFlyingEnemyController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	if (_behaviorTreeComponent->GetCurrentTree() == nullptr)
	{
		UScreenLogger::WriteError(" Constructor No behavior tree assigned to the component of the controller");
	}
	else
	{
		UScreenLogger::WriteSuccess(" Constructor behavior tree assigned to the component of the controller");
	}

	if (_customBehaviorTree == nullptr)
	{
		UScreenLogger::WriteError(" Constructor No behavior tree assigned to the controller");
	}
	else {
		UScreenLogger::WriteSuccess(" Constructor behavior tree assigned to controller");
	}
}

void AFlyingEnemyController::BeginPlay()
{
    Super::BeginPlay();

	RunBehaviorTreeCustomMatthias();
}

void AFlyingEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UBlackboardComponent* lBlackboardComp = nullptr;

	if (_behaviorTreeComponent->GetCurrentTree() == nullptr)
	{
		UScreenLogger::WriteError(" OnPossess No behavior tree assigned to the component of the controller");
	}
	else
	{
		UScreenLogger::WriteSuccess(" OnPossess behavior tree assigned to the component of the controller");
	}

	if (InPawn == nullptr)
	{
		UScreenLogger::WriteError(" OnPossess No pawn assigned to the controller");
	}
	else 
	{

		UScreenLogger::WriteSuccess(InPawn->GetName());
	}
	if (_customBehaviorTree == nullptr)
	{
		UScreenLogger::WriteError(" OnPossess No behavior tree assigned to the controller");
		return;
	}

	UScreenLogger::WriteSuccess("Possessing enemy controller");
    /*if (UseBlackboard(_behaviorTree->BlackboardAsset, lBlackboardComp))
    {
		Blackboard = lBlackboardComp;
        Blackboard->SetValueAsVector("TargetLocation", FVector::ZeroVector);
    }*/
}

void AFlyingEnemyController::RunBehaviorTreeCustomMatthias()
{

	if (_behaviorTreeComponent->GetCurrentTree() == nullptr)
	{
		UScreenLogger::WriteError(" RunBehaviorTree No behavior tree assigned to the component of the controller");
	}
	else
	{
		UScreenLogger::WriteSuccess(" RunBehaviorTree behavior tree assigned to the component of the controller");
	}

	if (_customBehaviorTree)
	{
		UScreenLogger::WriteSuccess("Run BehaviorTree");
		Super::RunBehaviorTree(_customBehaviorTree);
	}
	else
	{
		UScreenLogger::WriteError("RunBehaviorTree No behavior tree assigned to the controller");
	}
}