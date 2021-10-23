// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikeTrap.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AWizard;

UCLASS()
class DRAGONS_API ASpikeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
	float Damage;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Activate();

	
	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere,  Category = "Gameplay")
	float DeactivePosition;
	UPROPERTY(VisibleAnywhere,  Category = "Gameplay")
	float ActivePosition;
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bActive;
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bCanDamage;
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float TrapActiveVelocity;
	UPROPERTY(EditAnywhere, Category = "Gameplay");
	float TrapTiming = 4.f;



	UFUNCTION()
	void ResetCooldown();
	
	//Timer Handle to loop the SpikeTrap
	UPROPERTY(VisibleAnywhere, Category = "TimerHandles")
	FTimerHandle TimerHandleForTrap;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandles")
	FTimerHandle TimerHandleForPlayerCooldown;







};
