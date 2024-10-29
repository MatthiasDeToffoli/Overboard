// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ActorBuilder.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UActorBuilder : public UEngine
{
	GENERATED_BODY()

public:
	/**
	 * Create a subobject added to the character
	 *
	 * @param pParent component parent of the subobject
	 * @param pName name of the suboject
	 *
	 * @tparam TSubObjectType type of the suboject to create
	 *
	 * @return suboject created
	 */
	template<class TSubObjectType>
	static TSubObjectType* CreateSubObjects(AActor* pCaller, USceneComponent* pParent, FName pName);
	
};
