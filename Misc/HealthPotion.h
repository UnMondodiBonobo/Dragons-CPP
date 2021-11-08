// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPotion.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class DRAGONS_API AHealthPotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPotion();

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* InteractibleSphere;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
