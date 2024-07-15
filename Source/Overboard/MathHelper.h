// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OVERBOARD_API MathHelper
{
public:
	/**
	* Cross product between int
	* pA / pB, pC / pD we want pD
	* @param pA first number
	* @param pB second number
	* @param pC third number
	* 
	* @return the last number
	*/
	static int CrossProduct(int pA, int pB, int pC);

	/**
	* Cross product between int
	* pA / pB, pC / pD we want pD
	* @param pA first number
	* @param pB second number
	* @param pC third number
	*
	* @return the last number
	*/
	static float CrossProduct(float pA, float pB, float pC);

	/**
	* Cross product between int
	* pA / pB, pC / pD we want pD
	* @param pA first number
	* @param pB second number
	* @param pC third number
	*
	* @return the last number
	*/
	static double CrossProduct(double pA, double pB, double pC);

	/**
	* Cross product between int
	* pA / pB, pC / pD we want pC
	* @param pA first number
	* @param pB second number
	* @param pC last number
	*
	* @return the third number
	*/
	static int InvertedCrossProduct(int pA, int pB, int pD);

	/**
	* Cross product between int
	* pA / pB, pC / pD we want pC
	* @param pA first number
	* @param pB second number
	* @param pC last number
	*
	* @return the third number
	*/
	static float InvertedCrossProduct(float pA, float pB, float pD);

	/**
	* Cross product between int
	* pA / pB, pC / pD we want pC
	* @param pA first number
	* @param pB second number
	* @param pC last number
	*
	* @return the third number
	*/
	static double InvertedCrossProduct(double pA, double pB, double pD);

	/**
	* Check if the sign is different between two numbers
	* 
	* @param pA first number
	* @param pB second number
	* 
	* @return true if the sign is different, false otherwise
	*/
	static bool CheckSignDifference(int pA, int pB);

	/**
	* Check if the sign is different between two numbers
	*
	* @param pA first number
	* @param pB second number
	*
	* @return true if the sign is different, false otherwise
	*/
	static bool CheckSignDifference(float pA, float pB);

	/**
	* Check if the sign is different between two numbers
	*
	* @param pA first number
	* @param pB second number
	*
	* @return true if the sign is different, false otherwise
	*/
	static bool CheckSignDifference(double pA, double pB);

	/**
	* Get the angle between two vectors
	* 
	* @param pA First vector
	* @param pB second vector
	* @param pAsRadiant if we return the value as radiant or degrees
	* 
	* @return the angle between the two vectors
	*/
	static double AngleBetweenVectors(FVector pA, FVector pB, const bool pAsRadiant = false);
};
