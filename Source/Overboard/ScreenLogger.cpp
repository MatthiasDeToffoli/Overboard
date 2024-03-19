// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenLogger.h"

void UScreenLogger::WriteSuccess(FString pText) 
{
	WriteOnScreen(FColor::Green, pText);
}

void UScreenLogger::WriteInfo(FString pText) 
{
	WriteOnScreen(FColor::White, pText);
}


void UScreenLogger::WriteWarning(FString pText) 
{
	WriteOnScreen(FColor::Orange, pText);
}


void UScreenLogger::WriteError(FString pText) 
{
	WriteOnScreen(FColor::Red, pText);
}

void UScreenLogger::WriteOnScreen(float pNumber, int pNbDigits)
{
	WriteInfo(FString::SanitizeFloat(pNumber, pNbDigits));
}


void UScreenLogger::WriteOnScreen(int pNumber) 
{
	WriteInfo(FString::FromInt(pNumber));
}

void UScreenLogger::WriteOnScreen(FColor pColor, FString pText)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, pColor, pText);
	}
}