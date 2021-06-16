/// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USphereComponent;
class UParticleSystem;
class UPointLightComponent;
class APawn;
class AController;

UCLASS()
class DRAGONS_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneComp", meta = (AllowPrivateAccess = true))
	USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileParticleSystem",  meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* ProjectileParticleSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess = true))
	float ProjectileMaxSpeed = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess = true))
	float ProjectileInitialSpeed = 3000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileSphereComp", meta = (AllowPrivateAccess = true))
	USphereComponent* ProjectileSphereComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DespawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpellLight", meta = (AllowPrivateAccess = true))
	UPointLightComponent* SpellLight;

	UPROPERTY(EditAnywhere, Category = "SpellDamage")
	float SpellDamage;


	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	APawn* OwnerPawn;

	AController* PlayerController;


	
};
