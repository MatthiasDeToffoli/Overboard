// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScoreData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct OVERBOARD_API FScoreData
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString _name;
	UPROPERTY(EditAnywhere)
	FString _date;

public:
	UPROPERTY(BlueprintReadWrite)
	int Score;

public:
	FScoreData();
	FScoreData(const FString pName, const int pScore);
	~FScoreData();

public:
	FString ToString() const;
};
