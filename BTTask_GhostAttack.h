// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GhostAttack.generated.h"

class AGhost;

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTTask_GhostAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GhostAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	
	UPROPERTY()
	AGhost* Ghost;

	
};
