// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Uruk.h"
#include "AIController.h"

class AUruk;

UBTTask_Attack::UBTTask_Attack() 
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AUruk* Uruk = Cast<AUruk>(OwnerComp.GetAIOwner()->GetPawn());
    if(Uruk != nullptr) 
    {
        Uruk->Attack();
        return EBTNodeResult::Succeeded;
    }
    UE_LOG(LogTemp, Warning, TEXT("ATTACK DIDN'T CALL"));
    return EBTNodeResult::Failed;
}
