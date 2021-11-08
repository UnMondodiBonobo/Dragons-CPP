// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerAttacking.h"
#include "Kismet/GameplayStatics.h"
#include "Uruk.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"


UBTService_IsPlayerAttacking::UBTService_IsPlayerAttacking() 
{
    NodeName = TEXT("IS Player Attacking?");
    Contatore = 0;
}

void UBTService_IsPlayerAttacking::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* UrukPawn = OwnerComp.GetAIOwner()->GetPawn();
    AUruk* Uruk = Cast<AUruk>(UrukPawn);
    if(Uruk) 
    {
        bool isBeenHit = Uruk->bIsBeenHit;
        if(isBeenHit == true) 
        {
            
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
            
        }
    }
}
