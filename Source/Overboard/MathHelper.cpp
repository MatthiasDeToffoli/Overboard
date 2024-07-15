// Fill out your copyright notice in the Description page of Project Settings.


#include "MathHelper.h"

int MathHelper::CrossProduct(int pA, int pB, int pC)
{
	return (int)CrossProduct((double)pA, (double)pB, (double)pC);
}

float MathHelper::CrossProduct(float pA, float pB, float pC)
{
	return (float)CrossProduct((double)pA, (double)pB, (double)pC);
}

double MathHelper::CrossProduct(double pA, double pB, double pC)
{
	return (pB * pC) / pA;
}

int MathHelper::InvertedCrossProduct(int pA, int pB, int pD)
{
	return (int)InvertedCrossProduct((double)pA, (double)pB, (double)pD);
}

float MathHelper::InvertedCrossProduct(float pA, float pB, float pD)
{
	return (float)InvertedCrossProduct((double)pA, (double)pB, (double)pD);
}

double MathHelper::InvertedCrossProduct(double pA, double pB, double pD)
{
	return (pA * pD) / pB;
}

bool MathHelper::CheckSignDifference(int pA, int pB)
{
	return CheckSignDifference((double)pA, (double)pB);
}

bool MathHelper::CheckSignDifference(float pA, float pB)
{
	return CheckSignDifference((double)pA, (double)pB);
}

bool MathHelper::CheckSignDifference(double pA, double pB)
{
	return pA * pB < 0;
}

double MathHelper::AngleBetweenVectors(FVector pA, FVector pB, const bool pAsRadiant)
{
	double lDotProduct = FVector::DotProduct(pA.GetSafeNormal(), pB.GetSafeNormal());
	double lAngle = FMath::Acos(lDotProduct);

	if (pAsRadiant)
	{
		return lAngle;
	}

	return FMath::RadiansToDegrees(lAngle);
}
