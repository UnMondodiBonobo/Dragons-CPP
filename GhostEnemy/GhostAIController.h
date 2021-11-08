// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class DRAGONS_API AGhostAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	UBehaviorTree* GhostBehaviorTree;
	
};
