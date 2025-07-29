// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct OVERBOARD_API ScoreData
{
private :

	FString _name;
	FString _date;

public:
	int Score;

public:
	ScoreData(const FString pName, const int pScore);

public:
	FString ToString() const;
};
