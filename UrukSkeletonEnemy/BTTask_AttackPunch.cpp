// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPunch.h"
#include "Uruk.h"
#include "AIController.h"


UBTTask_AttackPunch::UBTTask_AttackPunch() 
{
    NodeName = TEXT("Attack Punch");
}

EBTNodeResult::Type UBTTask_AttackPunch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AUruk* Uruk = Cast<AUruk>(OwnerComp.GetAIOwner()->GetPawn());
    if(Uruk) 
    {
        Uruk->AttackPunch();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
