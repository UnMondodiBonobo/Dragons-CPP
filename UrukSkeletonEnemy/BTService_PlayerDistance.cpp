// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDistance.h"
#include "Ghost.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard.h"
#include "NavigationSystem.h"


UBTService_PlayerDistance::UBTService_PlayerDistance() 
{
    NodeName = TEXT("Player Distance");
}

void UBTService_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
    AGhost* Ghost = Cast<AGhost>(OwnerComp.GetAIOwner()->GetPawn());
    FVector GhostLocation = Ghost->GetActorLocation();
    AWizard* Wizard = Cast<AWizard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if(Ghost && Wizard) 
    {
       float Distance = FVector::Dist(PlayerLocation, GhostLocation);
       if(Distance <400.f) //if the player is too close, go back and calculate a safe position to shoot from
       {
           UE_LOG(LogTemp, Warning, TEXT("Distance not ok"));
           FVector SafeLocation = Ghost->GetActorLocation() + Ghost->GetActorForwardVector()*-600.f;
           FVector Offset(0.f, 0.f, 0.f);
           FNavLocation NavLocation;
           UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
           if(NavSystem) 
           {
               bool IsNavigable = NavSystem->ProjectPointToNavigation(SafeLocation, NavLocation, Offset);
               if(IsNavigable) //if safe location is in NavMesh range then set the value of SafeLocation
                {
                //DrawDebugSphere(GetWorld(), SafeLocation, 100.f, 12, FColor::Green, false, 1.f);
                UE_LOG(LogTemp, Warning, TEXT("Distance not ok %f, %f, %f"), SafeLocation.X, SafeLocation.Y, SafeLocation.Z);
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SafeLocation); 
                }
                else//if safe location is not in NavMesh then calculate a random point in navigable radius
                {
                FVector RandomLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), GhostLocation, 2000.f);
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation); 
                }
            }
            
            if(Distance < 200.f) 
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("MustStab"), true);
            }
            else 
            {
                OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("MustStab")); 
            }
       }
       else 
       {
           OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); 
           OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("MustStab")); 
       }
    }
}
