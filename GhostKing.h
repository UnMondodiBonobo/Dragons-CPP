// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostKing.generated.h"

class USceneComponent;
class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class DRAGONS_API AGhostKing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostKing();

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* InteractibleSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Text = FString(TEXT("Take this sword, take it... And send them back in the forgotten past where they came from"));

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SpawnGhostDialogue();

	UFUNCTION()
	void DespawnGhostDialogue();

	UFUNCTION()
	void AddTextCounter();
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsDialogueAlreadySpawned = false;

	UPROPERTY(BlueprintReadOnly)
	int TextCounter = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> BlueprintGhostDialogue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* GhostDialogue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	

	


};
