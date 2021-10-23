// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UrukShield.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class DRAGONS_API AUrukShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUrukShield();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void ActiveParry();

	UFUNCTION()
	void DeactiveParry();

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bIsParry;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
