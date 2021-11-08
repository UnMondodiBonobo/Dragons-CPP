// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostKnife.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class DRAGONS_API AGhostKnife : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostKnife();

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* KnifeMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* KnifeBoxCollider;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Stab();

	UFUNCTION()
	void StopStab();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	float KnifeLightDamage = 10.f;

	UPROPERTY()
	bool bIsKnifeStabbing = false;
};
