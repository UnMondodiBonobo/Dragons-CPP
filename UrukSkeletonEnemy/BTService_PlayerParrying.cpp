// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerParrying.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_PlayerParrying::UBTService_PlayerParrying() 
{
    NodeName = TEXT("Is Player Parrying");
}

void UBTService_PlayerParrying::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    AWizard* PlayerPawn = Cast<AWizard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if(PlayerPawn == nullptr) {return;}
    if(PlayerPawn->bIsParry) 
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
    }

    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}


