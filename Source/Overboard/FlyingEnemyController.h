// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FlyingEnemyController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class OVERBOARD_API AFlyingEnemyController : public AAIController
{
	GENERATED_BODY()
	
public :
    AFlyingEnemyController();

protected :
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> _customBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> _behaviorTreeComponent;

protected:
    virtual void BeginPlay() override;

public:
    virtual void OnPossess(APawn* InPawn) override;

    void RunBehaviorTreeCustomMatthias();
};
