// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearParryValue.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTTask_ClearParryValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ClearParryValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
