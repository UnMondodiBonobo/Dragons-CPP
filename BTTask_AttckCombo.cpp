// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttckCombo.h"
#include "Uruk.h"
#include "AIController.h"



UBTTask_AttckCombo::UBTTask_AttckCombo() 
{
    NodeName = TEXT("Attack Combo");
}

EBTNodeResult::Type UBTTask_AttckCombo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AUruk* Uruk = Cast<AUruk>(OwnerComp.GetAIOwner()->GetPawn());
    if(Uruk != nullptr) 
    {
        Uruk->AttackCombo();
        return EBTNodeResult::Succeeded;
    }
    UE_LOG(LogTemp, Warning, TEXT("ATTACKCOMBO DIDN'T CALL"));
    return EBTNodeResult::Failed;
}
