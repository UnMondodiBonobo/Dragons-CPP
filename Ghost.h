// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ghost.generated.h"

class AGhostSpell;
class AGhostKnife;

UCLASS()
class DRAGONS_API AGhost : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGhost();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	virtual float TakeDamage( float DamageAmount,struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION()
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void StopAttack();

	UFUNCTION(BlueprintCallable)
	void SpawnSpell();

	UFUNCTION()
	void StartStab();

	UFUNCTION(BlueprintCallable)
	void StopStab();

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStabbing = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsBeenHit = false;

	UFUNCTION(BlueprintCallable)
	void ResetHit();

	UPROPERTY(BlueprintReadWrite)
	float Health = MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(VisibleAnywhere)
	AGhostSpell* Spell;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGhostSpell> BlueprintSpellClass;

	UPROPERTY(VisibleAnywhere)
	AGhostKnife* Knife;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGhostKnife> BlueprintKnifeClass;

	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.f;



};
