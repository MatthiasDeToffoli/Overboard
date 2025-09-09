// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ScoreData.h"
#include "CustomSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FScoreData> SavedScores;
};
