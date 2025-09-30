#include "ScoreData.h"

FScoreData::FScoreData()
{
	name_ = "";
	score_ = 0;
	date_ = "";
}

FScoreData::FScoreData(const FString pName, const int pScore)
{
	name_ = pName;
	score_ = pScore;
	date_ = FDateTime::Now().ToString(TEXT("%Y-%m-%d"));
}

FString FScoreData::ToString() const
{
	return FString::Printf(TEXT("%s - %d - %s"), *name_, score_, *date_);
}


FScoreData::~FScoreData()
{
	name_ = NULL;
	score_ = NULL;
	date_ = NULL;
}
