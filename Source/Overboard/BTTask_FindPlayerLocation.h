#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayerLocation.generated.h"

/**
 * Task finding the player locatiob
 */
UCLASS()
class OVERBOARD_API UBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	/**
	* Default constructor
	*/
	UBTTask_FindPlayerLocation();

protected:
	/**
	* Execute this task
	* 
	* @param pOwnerComp The behavior tree component that owns this task
	* @param pNodeMemory Memory block allocated for this node
	* 
	* @return The result of executing the task Succeeded, Failed or InProgress
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory) override;
};
