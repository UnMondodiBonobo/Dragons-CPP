// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameModeBase.h"
#include "KillThemAllGameMode.generated.h"

class AUruk;

/**
 * 
 */
UCLASS()
class DRAGONS_API AKillThemAllGameMode : public AMyGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled)override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TimerToDeathScreen();

	FTimerHandle TimerHandleToDeathScreen;
	FTimerHandle TimerToSpawnUruk;

	UFUNCTION()
	void SpawnUruk();

	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AUruk> BlueprintUruk;

	TArray<FVector> RandomVectors;
	FVector Location;
    FVector Location2;
    FVector Location3;
	
};
