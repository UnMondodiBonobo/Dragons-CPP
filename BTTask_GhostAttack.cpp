// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GhostAttack.h"
#include "Ghost.h"
#include "AIController.h"


UBTTask_GhostAttack::UBTTask_GhostAttack() 
{
    NodeName = TEXT("GhostAttack");
}

EBTNodeResult::Type UBTTask_GhostAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    Ghost = Cast<AGhost>(OwnerComp.GetAIOwner()->GetPawn());
    if(Ghost) 
    {
        Ghost->StartAttack();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
