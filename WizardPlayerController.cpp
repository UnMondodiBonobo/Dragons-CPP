// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardPlayerController.h"
#include "Blueprint/UserWidget.h"


void AWizardPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    UE_LOG(LogTemp, Warning, TEXT("GAME HAS FINISHED"));

    LooseScreen = CreateWidget(this, BlueprintLooseScreen);
    if(LooseScreen) 
    {
        UI->RemoveFromViewport();
        LooseScreen->AddToViewport();
    }
    
    GetWorldTimerManager().SetTimer(TimerHandleToRestartGame, this, &APlayerController::RestartLevel, RestartDelay);
    
}

void AWizardPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    UI = CreateWidget(this, BlueprintUI);
    if(UI) 
    {
        UI->AddToViewport();
    }
    

}

