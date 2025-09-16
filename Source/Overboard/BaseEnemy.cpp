#include "BaseEnemy.h"
#include "Components/SphereComponent.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardPlayer.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
	
	_CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision's box");
	if (_mainContainer)
	{
		_CollisionComponent->SetupAttachment(_mainContainer);
	}

	_CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnBeginOverlap);
}

void ABaseEnemy::OnBeginOverlap(UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 pOtherBodyIndex, bool pFromSweep, const FHitResult& pSweepResult)
{
	if (pOtherActor->IsA(AOverboardPlayer::StaticClass()))
	{
		AOverboardPlayer* lTarget = Cast<AOverboardPlayer>(pOtherActor);

		if (lTarget)
		{
			UGameplayStatics::ApplyDamage(lTarget, _damage, nullptr, this, nullptr);
		}

		Destroy();
	}
}