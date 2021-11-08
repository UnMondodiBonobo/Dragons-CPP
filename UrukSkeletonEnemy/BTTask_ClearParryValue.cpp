// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearParryValue.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearParryValue::UBTTask_ClearParryValue() 
{
    NodeName = TEXT("Clear Parry Value");
}

EBTNodeResult::Type UBTTask_ClearParryValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    return EBTNodeResult::Succeeded;
}
