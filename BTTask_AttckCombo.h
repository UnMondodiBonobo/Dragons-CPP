// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttckCombo.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTTask_AttckCombo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttckCombo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
