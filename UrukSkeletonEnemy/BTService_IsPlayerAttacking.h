// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerAttacking.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API UBTService_IsPlayerAttacking : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsPlayerAttacking();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void ResetBlackboardValue();

	FTimerHandle TimerHandleToResetBeenAttacked;
	int32 Contatore;
	
};
