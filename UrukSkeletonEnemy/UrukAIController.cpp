// Fill out your copyright notice in the Description page of Project Settings.


#include "UrukAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"







void AUrukAIController::BeginPlay() 
{
    Super::BeginPlay();
    if(BehaviorTree != nullptr) 
    {
       
        RunBehaviorTree(BehaviorTree);
    }
}


void AUrukAIController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

}
