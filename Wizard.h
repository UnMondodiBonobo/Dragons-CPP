// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Wizard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBookDelegate, bool, IsMoving);

class UCharacterMovementComponent;
class UCapsuleComponent;
class USphereComponent;
class UCameraComponent;
class ASpell;
class AGlowingOrb;
class ASword;
class UMatineeCameraShake;
class AGhostKing;
class AHealthPotion;
class UPhysicsHandleComponent;
class UAnimationSequence;

UCLASS()
class DRAGONS_API AWizard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizard();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellLocation", meta = (AllowPrivateAcces = true))
	USceneComponent* SpellLocation;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* InteractibleSphere;

	UPROPERTY(EditAnywhere, Category = "Components")
	UPhysicsHandleComponent* TelekinesisHandle;

	UPROPERTY(editAnywhere, Category = "Components")
	USceneComponent* TelekinesisLocation;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FBookDelegate BookDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsSprinting = false;

	//To manage Parry
	UPROPERTY(BlueprintReadOnly)
	bool bIsParry = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStunned = false;

	//To manage if has been attacked and he parryied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsParryAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsBeenHit = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttackingNow = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttackingAnimation = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCharging = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCharged = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsChargedWithOrb = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsChargingWithOrb = false;

	//if can cast again
	UPROPERTY(BlueprintReadOnly)
	bool bIsCasting = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTelekinesis = false;

	//Check if attack is rejected
	UPROPERTY(BlueprintReadOnly)
	bool bIsRejected = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsBookMoving = false;

	UPROPERTY(BlueprintReadOnly)
	int32 Combo = -1;

	UPROPERTY(BlueprintReadOnly)
	ASword* Sword;

	UPROPERTY(BlueprintReadOnly)
	AGlowingOrb* NewGlowingOrb;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsTelekinesisLogicStillWorking = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCastingLogicStillWorking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Gameplay")
	bool bIsSpellEquipped = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bIsTelekinesisEquipped = false;

	UPROPERTY(BlueprintReadOnly)
	int32 DashCounter = 2;

	UFUNCTION()
	void ParryBroken(FVector Location);

	UFUNCTION(BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintPure)
	float GetMana();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	void ReleaseAttack();

	UFUNCTION(BlueprintCallable)
	void CallCast();

	UFUNCTION(BlueprintCallable)
	void ResetStunned();

	UFUNCTION(BlueprintCallable)
	void ResetCast();

	UFUNCTION(BlueprintCallable)
	void ResetTelekinesisLogic();

	UFUNCTION(BlueprintCallable)
	void ResetIsBeenHit();

	UFUNCTION(BlueprintCallable)
	void ResetIsReceivingAttack();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage( float DamageAmount,struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	bool bIsInteractibleDialogue = false;

	UPROPERTY()
	bool bIsInteractiblePotion = false;

	UFUNCTION()
	void Interact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Landed(const FHitResult& Hit) override;

private:

	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void DoubleJump();
	UFUNCTION()
	void StartRun();
	UFUNCTION()
	void StopRun();
	UFUNCTION()
	void Dash();
	UFUNCTION()
	void ResetDash();
	UFUNCTION()
	void ResetAttackAnimation();
	UFUNCTION()
	void HandAttack();
	UFUNCTION()
	void CastAttack();
	UFUNCTION()
	void PreCallOrb();
	UFUNCTION()
	void CallOrb();
	UFUNCTION()
	void StartParry();
	UFUNCTION()
	void StopParry();
	UFUNCTION()
	void ResetGroundFriction();
	UFUNCTION()
	void SetSpell();
	UFUNCTION()
	void SetTelekinesis();
	UFUNCTION()
	void ResetCastLogic();
	UFUNCTION()
	void DrinkHealthPotion();
	UFUNCTION()
	void CheckChargedAttack();



	UPROPERTY(EditAnywhere,  Category = "Gameplay", meta = (AllowPrivateAccess = true))
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	float RunSpeed = 550.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay", meta = (AllowPrivateAcces = true))
	float ParrySpeed = 250.f;

	UPROPERTY(EditAnywhere,  Category = "Gameplay", meta = (AllowPrivateAccess = true)) 
	int DoubleJumpCounter = 0;

	UPROPERTY(EditAnywhere,  Category = "Gameplay", meta = (AllowPrivateAccess = true))
	float JumpHeight = 550.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	float DashLength = 2500.f;

	//To Manage jump in blueprint
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bJumping = false;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float GroundFrictionResetTime;

	UPROPERTY(VisibleAnywhere)
	bool bHasTelekinesisGrabbed = false;

	UPROPERTY(VisibleAnywhere)
	AActor* TelekinesisHit;

	UPROPERTY(VisibleAnywhere)
	AGhostKing* GhostKing;

	UPROPERTY(VisibleAnywhere)
	AHealthPotion* HealthPotion;

	UPROPERTY(VisibleAnywhere)
	FVector End;

	UPROPERTY(VisibleAnywhere)
	FVector DefinitiveLocation;

	UPROPERTY(VisibleAnywhere)
	FRotator DefinitiveRotation;

	UPROPERTY(VisibleAnywhere)
	float Distance;

	UPROPERTY(VisibleAnywhere)
	FRotator ObjectDefaultRotation;


	UPROPERTY(EditDefaultsOnly)
	UCharacterMovementComponent* MovementComp;

	//Blueprint for spell
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpell> BlueprintSpellClass;

	//Blueprint for orb
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGlowingOrb> BlueprintGlowingOrb;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> BlueprintSword;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DespawnOrbParticle;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TelekinesisObjectNewMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TelekinesisObjectDefaultMaterial;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float MaxMana = 100.f;

	UPROPERTY(EditAnywhere)
	float ManaRechargeValue = 0.05f;

	UPROPERTY(VisibleAnywhere)
	bool bIsOrbTakingMana = false;

	UPROPERTY(EditDefaultsOnly)
	float Mana;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float HealthPotionRecharge = 40.f;

	UPROPERTY(VisibleAnywhere)
	bool bIsDialogueSpawned = false;




	//To Manage the dash direction
	UPROPERTY()
	bool Forward = false;
	UPROPERTY()
	bool Right = false;
	UPROPERTY()
	bool Left = false;
	UPROPERTY()
	bool bCanDash = true;
	
	//to handle glowing orb cooldown
	UPROPERTY()
	bool bIsGlowing = false;

	FTimerHandle TimerHandleForDash;
	FTimerHandle TimerHandleForCast;
	FTimerHandle TimerHandleToCallOrb;
	FTimerHandle TimerHandleForResetAttack;
	FTimerHandle TimerHandleForGroundFriction;
	FTimerHandle TimerHandleForResetCastLogic;
	FTimerHandle TimerHandleToCheckChargedAttack;

	//SOUNDS

	UPROPERTY(EditAnywhere)
	USoundBase* SpellCast;


	//CAMERA SHAKES

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> CameraShake;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> HitCameraShake;

	//ANIMATIONS
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation1;
	
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation2;
	
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation3;
	
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation4;
	
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation5;
	
	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation6;


	

};