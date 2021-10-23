// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"


void AGhostAIController::BeginPlay() 
{
    Super::BeginPlay();
    if(GhostBehaviorTree) 
    {
       RunBehaviorTree(GhostBehaviorTree);
    }
}

void AGhostAIController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
}
