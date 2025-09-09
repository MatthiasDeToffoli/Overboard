// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreData.h"

FScoreData::FScoreData()
{
	_name = "";
	Score = 0;
	_date = "";
}

FScoreData::FScoreData(const FString pName, const int pScore)
{
	_name = pName;
	Score = pScore;
	_date = FDateTime::Now().ToString(TEXT("%Y-%m-%d"));
}

FString FScoreData::ToString() const
{
	return FString::Printf(TEXT("%s - %d - %s"), *_name, Score, *_date);
}


FScoreData::~FScoreData()
{
	_name = NULL;
	Score = NULL;
	_date = NULL;
}
