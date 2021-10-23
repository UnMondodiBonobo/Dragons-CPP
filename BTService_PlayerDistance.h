// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTService_PlayerDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_PlayerDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
