// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTargetable.h"
#include "ActorBuilder.h"
#include <Kismet/KismetMathLibrary.h>
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

void ABaseTargetable::UpdateTargetRotation(FVector pPlayerPos, float pDeltaTime)
{
    FVector lCurrentLoc = GetActorLocation();

    // Calculate direction from the cube to the target actor
    FVector lDirectionToPlayer = pPlayerPos - lCurrentLoc;
    lDirectionToPlayer.Normalize();

    float lWidgetLocalDist = FVector::Distance(_mainMesh->GetRelativeLocation(), _targetWidgetComponent->GetRelativeLocation());
    FVector lNewWidgetPos = lCurrentLoc + lDirectionToPlayer * lWidgetLocalDist;

    // Set the widget's position
    _targetWidgetComponent->SetWorldLocation(lNewWidgetPos);

    // Make the widget face the target actor
    FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(pPlayerPos - lNewWidgetPos);
    _targetWidgetComponent->SetWorldRotation(TargetRotation);

}
