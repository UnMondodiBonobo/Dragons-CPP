// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Ghost.h"

class UCharacterMovementComponent;



UMyBTService_PlayerLocationIfSeen::UMyBTService_PlayerLocationIfSeen() 
{
    NodeName = TEXT("Player Location If Seen");
}

void UMyBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(AIController && PlayerPawn) 
    {
       bool IsPlayerSeen = AIController->LineOfSightTo(PlayerPawn);
        if(IsPlayerSeen) 
        {
            AGhost* Ghost = Cast<AGhost>(AIController->GetPawn());
            if(Ghost) 
            {
                AIController->SetFocus(PlayerPawn,EAIFocusPriority::Gameplay);//if this service is attached to ghost
            }
            FVector PlayerLocation = PlayerPawn->GetActorLocation();
            FVector PlayerOffset = PlayerPawn->GetActorForwardVector() * 1000.f;
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("Parry Location"), PlayerLocation + PlayerOffset);
        }
        else
        {
            OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
            OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("Parry Location"));
        }
    }
}
