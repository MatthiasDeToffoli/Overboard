// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTargetable.generated.h"

UCLASS()
class OVERBOARD_API ABaseTargetable : public AActor
{
	GENERATED_BODY()

private:
	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _mainContainer;

	UPROPERTY(EditAnywhere, Category = "Target")
	UStaticMeshComponent* _targetMesh;

	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graphism", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _graphismContainer;

	UPROPERTY(EditAnywhere, Category = "Graphism")
	UStaticMeshComponent* _mainMesh;
	
public:	
	// Sets default values for this actor's properties
	ABaseTargetable();

protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public :
	void SetTargeted(bool pIsTargeted);

};
