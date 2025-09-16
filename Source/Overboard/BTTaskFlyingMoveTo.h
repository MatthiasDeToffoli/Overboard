#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskFlyingMoveTo.generated.h"

class ABaseEnemy;

/**
 * Task to make the enemy fly to the target location
 */
UCLASS()
class OVERBOARD_API UBTTaskFlyingMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    /// <summary>
    /// Default constructor
    /// </summary>
    UBTTaskFlyingMoveTo();

private:
    /// <summary>
    /// Get the enemy pawn if it exists
    /// </summary>
    /// <param name="pOwnerComp">The owner behavior tree component</param>
    /// <returns>The pawn of class <see cref="ABaseEnemy"/>.></returns>
    ABaseEnemy* GetEnemyPawn(UBehaviorTreeComponent& pOwnerComp) const;

	/// <summary>
	/// Check if there is an obstacle in a direction
	/// </summary>
	/// <param name="pCurrentLocation">Current enemy pawn location</param>
	/// <param name="pDirection">Direction to check</param>
	/// <param name="pAvoidDistance">Distance the enemy as to stop from the obstacle</param>
	/// <param name="offset">Offset to see the aread the enemy check to avoid walls</param>
	/// <param name="pParams">Collision parameters</param>
	/// <returns><c>true</c> if there is an obstacle, <c>false</c> instead</returns>
	bool CheckHasObstacle(FVector pCurrentLocation, FVector pDirection, float pAvoidDistance, float offset, FCollisionQueryParams pParams);

    /// <summary>
    /// Check if there is an obstacle in a direction
    /// </summary>
    /// <param name="pCurrentLocation">Current enemy pawn location</param>
    /// <param name="pDirection">Direction to check</param>
    /// <param name="pAvoidDistance">Distance the enemy as to stop from the obstacle</param>
    /// <param name="pParams">Collision parameters</param>
    /// <returns><c>true</c> if there is an obstacle, <c>false</c> instead</returns>
    bool CheckHasObstacle(FVector pCurrentLocation, FVector pDirection, float pAvoidDistance, FCollisionQueryParams pParams);
protected:
    /// <summary>
    /// Execute the task
    /// </summary>
    /// <param name="pOwnerComp">The owner behavior tree component</param>
    /// <param name="pNodeMemory">Memory took by the current node</param>
    /// <returns>If the task succeed or not</returns>
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory) override;

    /// <summary>
    /// Tick of the task
    /// </summary>
    /// <param name="pOwnerComp">The owner behavior tree component</param>
    /// <param name="pNodeMemory">Memory took by the current node</param>
    /// <param name="pDeltaSeconds">delta seconds between two ticks</param>
    virtual void TickTask(UBehaviorTreeComponent& pOwnerComp, uint8* pNodeMemory, float pDeltaSeconds) override;

    /// <summary>
    /// Get the current instance memory size
    /// </summary>
    /// <returns>The current instance memory size</returns>
    virtual uint16 GetInstanceMemorySize() const override;

private:
    /// <summary>
    /// Location of the target to follow
    /// </summary>
    FVector targetLocation;
};
