// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerParrying.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTService_PlayerParrying : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_PlayerParrying();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
