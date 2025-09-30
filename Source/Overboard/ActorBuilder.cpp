#include "ActorBuilder.h"
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include <GameFramework/SpringArmComponent.h>

// Explicit templates instantiation for the types needed to avoid errors on the method CreateSubObjects
template USceneComponent* UActorBuilder::CreateSubObjects<USceneComponent>(AActor*, USceneComponent*, FName);
template UStaticMeshComponent* UActorBuilder::CreateSubObjects<UStaticMeshComponent>(AActor*, USceneComponent*, FName);
template USpringArmComponent* UActorBuilder::CreateSubObjects<USpringArmComponent>(AActor*, USceneComponent*, FName);
template UCapsuleComponent* UActorBuilder::CreateSubObjects<UCapsuleComponent>(AActor*, USceneComponent*, FName);
template UCameraComponent* UActorBuilder::CreateSubObjects<UCameraComponent>(AActor*, USceneComponent*, FName);
template UWidgetComponent* UActorBuilder::CreateSubObjects<UWidgetComponent>(AActor*, USceneComponent*, FName);
template USphereComponent* UActorBuilder::CreateSubObjects<USphereComponent>(AActor*, USceneComponent*, FName);

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

