// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UrukAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class DRAGONS_API AUrukAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;	

private:

	UPROPERTY(EditAnywhere, Category = "BehaviourTree")
	UBehaviorTree* BehaviorTree;

	


};
