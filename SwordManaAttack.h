// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordManaAttack.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UProjectileMovementComponent;

UCLASS()
class DRAGONS_API ASwordManaAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordManaAttack();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, Category = "Components");
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* Movement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	
	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	float SwordManaDamage;

};
