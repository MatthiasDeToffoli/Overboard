#include "BaseTargetable.h"
#include "ActorBuilder.h"
#include <Components/WidgetComponent.h>
#include "HealthComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "OverboardPlayerController.h"


ABaseTargetable::ABaseTargetable()
{
	PrimaryActorTick.bCanEverTick = false;

	mainContainer_ = UActorBuilder::CreateSubObjects<USceneComponent>(this,RootComponent, "Main container");
	targetWidgetComponent_ = UActorBuilder::CreateSubObjects<UWidgetComponent>(this, mainContainer_, "Target widget component");
	graphismContainer_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, mainContainer_, "Graphism container");
	mainMesh_ = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, graphismContainer_, "Main mesh");
	healthComponent_ = CreateDefaultSubobject<UHealthComponent>("Health");
}


void ABaseTargetable::BeginPlay()
{
	Super::BeginPlay();
	targetWidgetComponent_->SetVisibility(false, true);
	SetCanBeDamaged(true);
}

void ABaseTargetable::SetTargeted(bool pIsTargeted)
{
	targetWidgetComponent_->SetVisibility(pIsTargeted, true);
}

void ABaseTargetable::UpdateTargetRotation(FVector pPlayerPos, float pDeltaTime)
{
    FVector lCurrentLoc = GetActorLocation();

    // Calculate direction from the cube to the target actor
    FVector lDirectionToPlayer = pPlayerPos - lCurrentLoc;
    lDirectionToPlayer.Normalize();

    float lWidgetLocalDist = FVector::Distance(mainMesh_->GetRelativeLocation(), targetWidgetComponent_->GetRelativeLocation());
    FVector lNewWidgetPos = lCurrentLoc + lDirectionToPlayer * lWidgetLocalDist;

    // Set the widget's position
    targetWidgetComponent_->SetWorldLocation(lNewWidgetPos);

    // Make the widget face the target actor
    FRotator lTargetRotation = UKismetMathLibrary::MakeRotFromX(pPlayerPos - lNewWidgetPos);
    targetWidgetComponent_->SetWorldRotation(lTargetRotation);

}

float ABaseTargetable::TakeDamage(float pDamageAmount, FDamageEvent const& pDamageEvent, AController* pEventInstigator, AActor* pDamageCauser)
{
	if (healthComponent_->ApplyDamage(pDamageAmount))
	{
		if (AOverboardPlayerController* lPlayerCont = Cast<AOverboardPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			lPlayerCont->UpdateScore(scoreToGive_);
		}
		
		Destroy();
	}
	return pDamageAmount;
}
