// Fill out your copyright notice in the Description page of Project Settings.


#include "KillThemAllGameMode.h"
#include "Uruk.h"


void AKillThemAllGameMode::BeginPlay() 
{
    Super::BeginPlay();
    /*UE_LOG(LogTemp, Error, TEXT("SUCA"));
    GetWorldTimerManager().SetTimer(TimerToSpawnUruk, this, &AKillThemAllGameMode::SpawnUruk, 20.f, true);

    Location = FVector(-9005.f,541.f,25.f);
    Location2 = FVector(-8852.f,-439.f,69.f);
    Location3 = FVector(-6686.f,-128.f,100.f);*/
}

void AKillThemAllGameMode::SpawnUruk() 
{
    
    /*RandomVectors.Add(Location);
    RandomVectors.Add(Location2);
    RandomVectors.Add(Location3);
    int32 Random = FMath::RandRange(0, 2);

    FRotator Rotation(0.f, 0.f, 0.f);
    GetWorld()->SpawnActor<AUruk>(BlueprintUruk, RandomVectors.GetData()[Random], Rotation);*/
}

void AKillThemAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);
    PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    GetWorldTimerManager().SetTimer(TimerHandleToDeathScreen, this, &AKillThemAllGameMode::TimerToDeathScreen, 2.5f, false);
}

void AKillThemAllGameMode::TimerToDeathScreen() 
{
    if(PlayerController) 
    {
        PlayerController->GameHasEnded(nullptr, false);
    }
}
