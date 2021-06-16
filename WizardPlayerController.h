// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONS_API AWizardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> BlueprintLooseScreen;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> BlueprintUI;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* LooseScreen;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* UI;

	FTimerHandle TimerHandleToRestartGame;
	
};
