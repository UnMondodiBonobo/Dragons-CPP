// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomLocation.h"
#include "Uruk.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"



UBTTask_RandomLocation::UBTTask_RandomLocation() 
{
    NodeName = TEXT("Random Location");
}

EBTNodeResult::Type UBTTask_RandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AUruk* Uruk = Cast<AUruk>(OwnerComp.GetAIOwner()->GetPawn());
    if(Uruk) //If Uruk isn't nullptr execute
    {
        FVector Location = Uruk->GetActorLocation();//create a new fvector to store the random location
        FVector AvailableLocation = UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(GetWorld(), Location, 2000.f);
       OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), AvailableLocation);//set the blackboardvalue as the random location

       return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
