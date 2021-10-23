// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackPunch.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTTask_AttackPunch : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackPunch();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
