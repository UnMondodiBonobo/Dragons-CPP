// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Uruk.generated.h"

class UStaticMeshComponent;
class AUrukWeapon;
class AUrukShield;
class AWizard;
class AGlowingTurret;
class ASwordManaAttack;

UCLASS()
class DRAGONS_API AUruk : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUruk();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	AUrukWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AUrukWeapon> BlueprintWeapon;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	AUrukShield* Shield;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AUrukShield> BlueprintShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bIsAttackingCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsPunching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsParrying;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsBeenHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bKick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 Combo;


	UFUNCTION()
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void AttackCombo();

	UFUNCTION()
	void AttackPunch();

	UFUNCTION()
	void Detach();

	UFUNCTION()
	void Parry();

	UFUNCTION()
	bool GetIsBeenHit();

	UFUNCTION()
	void BreakPlayerParry();

	UFUNCTION(BlueprintCallable)
	void ResetKick();

	UFUNCTION(BlueprintCallable)
	void BreakParry();

	UFUNCTION(BlueprintCallable)
	void LaunchAttackCombo();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float PunchDamage;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float AreaDamage;

	UPROPERTY(VisibleAnywhere)
	APawn* Player;

	UPROPERTY(VisibleAnywhere)
	AWizard* Wizard;

	UPROPERTY(VisibleAnywhere)
	AGlowingTurret* Turret;

	UPROPERTY(VisibleAnywhere)
	ASwordManaAttack* SwordMana;

	FTimerHandle TimerHandleForAttack;
	FTimerHandle TimerHandleToResetParry;
	FTimerHandle TimerHandleForResetHit;
	FTimerDelegate TimerDel;
	FTimerDelegate TimerDel2;
	FTimerHandle TimerHandleForHitMovement;
	FTimerHandle TimerHandleForAttackCombo;
	FTimerHandle TimerHandleToPunchAttack;
	FTimerHandle TimerHandleToPunchDamage;
	FTimerHandle TimerHandleForPlayerMovement;

	UFUNCTION()
	float CalculateDistanceToPlayer();

	UFUNCTION()
	void Punch();

	UFUNCTION()
	void ResetAttack();

	UFUNCTION()
	void ResetParry();

	UFUNCTION()
	void ResetHit();

	UFUNCTION()
	void ResetAttackCombo();

	UFUNCTION()
	void MoveHit(FVector CleanDirection);

	UFUNCTION()
	void ResetAttackPunch();

	UFUNCTION()
	void MovePlayer(FVector Direction);


};
