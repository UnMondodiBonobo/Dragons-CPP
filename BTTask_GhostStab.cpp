// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GhostStab.h"
#include "Ghost.h"
#include "AIController.h"


UBTTask_GhostStab::UBTTask_GhostStab() 
{
    NodeName = TEXT("Stab");
}

EBTNodeResult::Type UBTTask_GhostStab::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    Ghost = Cast<AGhost>(OwnerComp.GetAIOwner()->GetPawn());
    if(Ghost) 
    {
        Ghost->StartStab();
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}


