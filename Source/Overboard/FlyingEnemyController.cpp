// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AFlyingEnemyController::AFlyingEnemyController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
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

    /*if (UseBlackboard(_behaviorTree->BlackboardAsset, lBlackboardComp))
    {
		Blackboard = lBlackboardComp;
        Blackboard->SetValueAsVector("TargetLocation", FVector::ZeroVector);
    }*/
}

void AFlyingEnemyController::RunBehaviorTreeCustomMatthias()
{

}