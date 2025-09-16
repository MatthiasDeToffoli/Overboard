#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ActorBuilder.generated.h"

/**
 * Use to avoid repeating some code present in most of the actors which can have different parents
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
	 * @return subobject created
	 */
	template<class TSubObjectType>
	static TSubObjectType* CreateSubObjects(AActor* pCaller, USceneComponent* pParent, FName pName);
	
};
