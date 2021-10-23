// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TelekinesisRay.generated.h"

UCLASS()
class DRAGONS_API ATelekinesisRay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATelekinesisRay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
