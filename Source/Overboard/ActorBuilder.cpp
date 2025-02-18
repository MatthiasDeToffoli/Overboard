// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBuilder.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Explicit template instantiation for the types needed-----------------------------------------------------------------------------------------------------------------------
template USceneComponent* UActorBuilder::CreateSubObjects<USceneComponent>(AActor*, USceneComponent*, FName);
template UStaticMeshComponent* UActorBuilder::CreateSubObjects<UStaticMeshComponent>(AActor*, USceneComponent*, FName);
template USpringArmComponent* UActorBuilder::CreateSubObjects<USpringArmComponent>(AActor*, USceneComponent*, FName);
template UCapsuleComponent* UActorBuilder::CreateSubObjects<UCapsuleComponent>(AActor*, USceneComponent*, FName);
template UCameraComponent* UActorBuilder::CreateSubObjects<UCameraComponent>(AActor*, USceneComponent*, FName);
template UWidgetComponent* UActorBuilder::CreateSubObjects<UWidgetComponent>(AActor*, USceneComponent*, FName);
template USphereComponent* UActorBuilder::CreateSubObjects<USphereComponent>(AActor*, USceneComponent*, FName);

//Methodes-----------------------------------------------------------------------------------------------------------------------
template<class TSubObjectType>
TSubObjectType* UActorBuilder::CreateSubObjects(AActor* pCaller, USceneComponent* pParent, FName pName)
{
	TSubObjectType* lObject = pCaller->CreateDefaultSubobject<TSubObjectType>(pName);

	if (lObject)
	{
		lObject->SetupAttachment(pParent);
	}

	return lObject;
}

