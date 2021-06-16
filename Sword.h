// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USoundBase;
class AWizard;
class UMaterial;
class UParticleSystem;
class ASwordManaAttack;

UCLASS()
class DRAGONS_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAcces = true))
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAcces = true))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAcces = true))
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsRejected;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsSwordEnchanted;

	UFUNCTION()
	void Attack();
	UFUNCTION()
	void StopAttack();
	UFUNCTION()
	void ManaEnchant();
	UFUNCTION()
	void SteelEnchant();
	UFUNCTION(BlueprintCallable)
	void SpawnSwordMana();
	/*UFUNCTION()
    void ParryAttack();*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	AWizard* Player;

	UPROPERTY(VisibleAnywhere)
	bool bCheckAttack;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LightAttackDamage;

	//Sword Sounds
	UPROPERTY(EditAnywhere)
	USoundBase* SwingSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	FTimerHandle TimerHandleForReject;

	UFUNCTION()
	void Reject();

	UFUNCTION()
	void ResetReject();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Sword Materials
	UPROPERTY(EditAnywhere)
	UMaterial* ManaMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* SteelMaterial;

	UPROPERTY(VisibleAnywhere)
	ASwordManaAttack* SwordMana;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASwordManaAttack> BlueprintSwordMana;







};
