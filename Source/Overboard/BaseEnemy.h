// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AIController.h>
#include "CoreMinimal.h"
#include "BaseTargetable.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API ABaseEnemy : public ABaseTargetable
{
	GENERATED_BODY()

public:
	/// <summary>
	/// if the enemy is stupid, it will not avoid walls
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool isStupid;

	/// <summary>
	/// Wall avoiding distance
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float wallAvoidingDistance = 300.f;

	/// <summary>
	/// Offset to see the aread the enemy check to avoid walls
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float avoidingOffsetValue = 50.f;

	/// <summary>
	/// Movement speed
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float speed = 6000.f;
};
