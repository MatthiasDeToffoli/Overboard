// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTargetable.h"
#include "ActorBuilder.h"
//#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseTargetable::ABaseTargetable()
{
 	// No necessary yet
	PrimaryActorTick.bCanEverTick = false;

	_mainContainer = UActorBuilder::CreateSubObjects<USceneComponent>(this,RootComponent, "Main container");
	_targetWidgetComponent = UActorBuilder::CreateSubObjects<UWidgetComponent>(this, _mainContainer, "Target widget component");
	_graphismContainer = UActorBuilder::CreateSubObjects<USceneComponent>(this, _mainContainer, "Graphism container");
	_mainMesh = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, _graphismContainer, "Main mesh");
}


void ABaseTargetable::BeginPlay()
{
	Super::BeginPlay();
	_targetWidgetComponent->SetVisibility(false, true);
}

void ABaseTargetable::SetTargeted(bool pIsTargeted)
{
	_targetWidgetComponent->SetVisibility(pIsTargeted, true);
}
