#pragma once

#include "CoreMinimal.h"

/**
 * All constants used in the game
 */
static class OVERBOARD_API Constants
{
public:
	static class BlackBoard
	{
	public:
		/*
		* Blackboard Key for the target location
		*/
		static const FName kTargetLocation();
	};

	static class Save
	{
	public:
		/**
		* Save file name
		*/
		static const FString  kSaveFileName();
	};
};
