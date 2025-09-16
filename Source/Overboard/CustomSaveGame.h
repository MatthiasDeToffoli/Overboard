#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomSaveGame.generated.h"

struct FScoreData;

/**
 * Custome save game to save player score
 */
UCLASS()
class OVERBOARD_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/**
	* All scores saved
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FScoreData> SavedScores;
};
