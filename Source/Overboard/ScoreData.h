#pragma once

#include "CoreMinimal.h"
#include "ScoreData.generated.h"

/**
 * Data structure to save or load score informations
 */
USTRUCT(BlueprintType)
struct OVERBOARD_API FScoreData
{
	GENERATED_BODY()

private:
	/**
	* Player's name
	*/
	UPROPERTY(EditAnywhere)
	FString name_;

	/**
	* Player's score
	*/
	UPROPERTY(EditAnywhere)
	int score_;

	/**
	* date of the save
	*/
	UPROPERTY(EditAnywhere)
	FString date_;

public:
	/**
	* Default constructor
	*/
	FScoreData();

	/**
	* Constructor setting name and score
	*/
	FScoreData(const FString pName, const int pScore);

	/**
	* Default destructor
	*/
	~FScoreData();

public:
	/**
	* Get a string representation of the score data
	*/
	FString ToString() const;
};
