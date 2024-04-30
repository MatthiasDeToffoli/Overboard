// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ScreenLogger.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UScreenLogger : public UEngine
{
	GENERATED_BODY()
	
public :
	/**
	 * Write a success log on screen.
	 *
	 * @param pText Text to log.
	 */
	static void WriteSuccess(FString pText);

	/**
	 * Write an info log on screen.
	 *
	 * @param pText Text to log.
	 */
	static void WriteInfo(FString pText);

	/**
	 * Write a warning log on screen.
	 *
	 * @param pText Text to log.
	 */
	static void WriteWarning(FString pText);

	/**
	 * Write an error log on screen.
	 *
	 * @param pText Text to log.
	 */
	static void WriteError(FString pText);

	/**
	 * Write an info float log on screen.
	 *
	 * @param pNumber float to log.
	 * @param pNbDigits number of digits to log
	 */
	static void WriteOnScreen(float pNumber, int pNbDigits = 1);

	/**
	 * Write an info int log on screen.
	 *
	 * @param pNumber int to log.
	 */
	static void WriteOnScreen(int pNumber);

	/**
	 * Write a log on screen.
	 *
	 * @param pColor color of the log.
	 * @param pText Text to log.
	 */
	static void WriteOnScreen(FColor pColor, FString pText);
};
