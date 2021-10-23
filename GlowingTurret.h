// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlowingPlatform.h"
#include "GlowingTurret.generated.h"


class USphereComponent;
class AUruk;
class ASpell;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class DRAGONS_API AGlowingTurret : public AGlowingPlatform
{
	GENERATED_BODY()
public:
	AGlowingTurret();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USceneComponent* ProjectileSpawnPoint;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
		// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandleForShoot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpell> BlueprintSpell;

	UPROPERTY()
	ASpell* Spell;

	UPROPERTY()
	AUruk* Uruk;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* SpawnParticle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DespawnParticle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AUruk> UrukBlueprint;

	UFUNCTION()
	void CheckFireCondition();

	UFUNCTION()
	void Shoot();
};