// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlowingOrb.generated.h"

class UParticleSystemComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class UCameraComponent;
class USphereComponent;


UCLASS()
class DRAGONS_API AGlowingOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlowingOrb();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneComp", meta = (AllowPrivateAccess = true))
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* SpawnParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DespwanParticleSystem",  meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* DespawnParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh",  meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Light", meta = (AllowPrivateAccess = true))
	UPointLightComponent* Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Light", meta = (AllowPrivateAccess = true))
	USphereComponent* SphereCollider;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
