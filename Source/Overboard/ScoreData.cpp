// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreData.h"

ScoreData::ScoreData(const FString pName, const int pScore)
{
	_name = pName;
	Score = pScore;
	_date = FDateTime::Now().ToString(TEXT("%Y-%m-%d"));
}

FString ScoreData::ToString() const
{
	return FString::Printf(TEXT("%s %d %s"), *_name, Score, *_date);
}
