// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UrukWeapon.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class AUruk;

UCLASS()
class DRAGONS_API AUrukWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUrukWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Reset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	bool IsAttacking;

	UPROPERTY(VisibleAnywhere)
	AUruk* UrukOwner;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LightAttackDamage;

	FTimerHandle TimerHandleForAttackDelay;

	TArray<AActor*> Enemies;

	UFUNCTION()
	void StartCheckingAttack();



	



};
