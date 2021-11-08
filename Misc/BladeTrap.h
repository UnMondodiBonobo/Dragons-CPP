// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladeTrap.generated.h"

class UBoxComponent;
class UPhysicsThrusterComponent;

UCLASS()
class DRAGONS_API ABladeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABladeTrap();

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* RotationalBlade;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshForRotation;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Components")
	UPhysicsThrusterComponent* Thruster;

	UPROPERTY(EditAnywhere, Category = "Components")
	UPhysicsThrusterComponent* Thruster2;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float Damage;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Swing();

	FTimerHandle TimerHandleToStartSwing;
	FTimerHandle TimerHandleForPlayerCooldown;

	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bCanDamage;

	UFUNCTION()
	void ResetCooldown();

};
