// Fill out your copyright notice in the Description page of Project Settings.


#include "Wizard.h"
#include "Spell.h"
#include "GlowingOrb.h"
#include "Sword.h"
#include "Camera/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShake.h"
#include "GhostKing.h"
#include "HealthPotion.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "AnimationBlueprintLibrary.h"
#include "Animation/AnimSequence.h"

// Sets default values
AWizard::AWizard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 1.f;
	
	//Creates Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("camsocket"));
	Camera->bUsePawnControlRotation = true;

	SpellLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpellLocation"));
	SpellLocation->SetupAttachment(GetCapsuleComponent());

	InteractibleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractibleSphere"));
	InteractibleSphere->SetupAttachment(GetRootComponent());

	TelekinesisHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("TelekinesisHandle"));

	TelekinesisLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TelekinesisLocation"));
	TelekinesisLocation->SetupAttachment(Camera);

	MovementComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	MovementComp->MaxWalkSpeed = WalkSpeed;

	SetCanBeDamaged(true);

}

// Called when the game starts or when spawned
void AWizard::BeginPlay()
{
	Super::BeginPlay();
	//Hide player head
	GetMesh()->HideBoneByName(FName("head"), EPhysBodyOp::PBO_None);
	//GetMesh()->HideBoneByName(FName("calf_r"), EPhysBodyOp::PBO_None);
	//GetMesh()->HideBoneByName(FName("calf_l"), EPhysBodyOp::PBO_None);

	Sword = GetWorld()->SpawnActor<ASword>(BlueprintSword);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("swordsocket"));
	Sword->SetOwner(this);

	Health = MaxHealth;
	Mana = MaxMana;

	InteractibleSphere->OnComponentBeginOverlap.AddDynamic(this, &AWizard::OnOverlapBegin);
	InteractibleSphere->OnComponentEndOverlap.AddDynamic(this, &AWizard::OnOverlapEnd);

}

// Called every frame
void AWizard::Tick(float DeltaTime)
{



	Super::Tick(DeltaTime);

	if(bHasTelekinesisGrabbed) //If Telekinesis grabbed something, subtract mana 
	{
		Mana -= 0.15f;
		FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("spellsocket"));
		FRotator PlayerRotation = Camera->GetComponentRotation();
		
		if(TelekinesisHit) //Safe access to Object hit by Telekinesis
		{
			End = HandLocation+PlayerRotation.Vector()*Distance;//Calculate Telekinesis Object position using the distance between Player and Object
			TelekinesisLocation->SetWorldLocation(End);
			DefinitiveLocation = TelekinesisLocation->GetComponentLocation();
			DefinitiveRotation = TelekinesisLocation->GetComponentRotation();
			DefinitiveRotation.Pitch = 0.f;
			DefinitiveRotation.Roll = 0.f;
			TelekinesisHandle->SetTargetLocationAndRotation(DefinitiveLocation, DefinitiveRotation);
			
			//Now check if we are making the object collide with something ex: wall, ground
			FHitResult Hit;
			FVector Start = GetMesh()->GetSocketLocation(TEXT("spellsocket"));
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			bool IsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, QueryParams);
			if(IsHit && Hit.GetActor() != TelekinesisHit) //If the raycast is colliding, then also the object itself is colliding: try to reset cast
			{
				UE_LOG(LogTemp, Error, TEXT("Object is colliding with: %s"), *Hit.GetActor()->GetName());
				ResetCast();
			}
		}
	}

	if(Mana <= 0.f) 
	{
		UE_LOG(LogTemp, Warning, TEXT("orb dovrebbe distruggersi E MANA A 0"));
		Mana = 0.f;
		Mana += ManaRechargeValue;//Adds RechargeValue to Mana each frame
		
		if(NewGlowingOrb && bIsGlowing)
		{
			PreCallOrb();
		}

		if(bHasTelekinesisGrabbed) //if mana is 0 or less then detach from telekinesis
		{
			ResetCast();
		}
	}
	else 
	{
		if(bIsOrbTakingMana) 
		{
			Mana -= 0.15f;//Mana to subtract while orb is casted
		}
		
		else 
		{
			if(Mana < MaxMana && (Mana+ManaRechargeValue)<=MaxMana) //Second parameter is needed because Mana can't increase more then MaxMana
			{
				Mana += ManaRechargeValue;//Adds RechargeValue to Mana each frame
			}
			else//If Recharge will increase mana over 100, then just set the mana to 100
			{
				Mana = MaxMana;
			}
		}
	}


}

// Called to bind functionality to input
void AWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWizard::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AWizard::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AWizard::DoubleJump);
	PlayerInputComponent->BindAction(TEXT("StartRun"), IE_Pressed, this, &AWizard::StartRun);
	PlayerInputComponent->BindAction(TEXT("StartRun"), IE_Released, this, &AWizard::StopRun);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AWizard::Dash);
	PlayerInputComponent->BindAction(TEXT("MouseSXAttack"), IE_Pressed, this, &AWizard::HandAttack);
	PlayerInputComponent->BindAction(TEXT("MouseSXAttackReleased"), IE_Released, this, &AWizard::ReleaseAttack);
	PlayerInputComponent->BindAction(TEXT("Cast"), IE_Pressed, this, &AWizard::CastAttack);
	PlayerInputComponent->BindAction(TEXT("Parry"), IE_Pressed, this, &AWizard::StartParry);
	PlayerInputComponent->BindAction(TEXT("Parry"), IE_Released, this, &AWizard::StopParry);
	PlayerInputComponent->BindAction(TEXT("Orb"), IE_Pressed, this, &AWizard::PreCallOrb);
	PlayerInputComponent->BindAction(TEXT("SpellEquipped"), IE_Pressed, this, &AWizard::SetSpell);
	PlayerInputComponent->BindAction(TEXT("TelekinesisEquipped"), IE_Pressed, this, &AWizard::SetTelekinesis);
	PlayerInputComponent->BindAction((TEXT("Interact")), IE_Pressed, this, &AWizard::Interact);

}







//                                                       TAKE DAMAGE
float AWizard::TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) 
{
	float Damage = 0.f;
	if(DamageCauser != nullptr) 
	{
		Damage = Super::TakeDamage(DamageAmount, DamageEvent, nullptr, DamageCauser);
		Damage = FMath::Min(Health, Damage);
		if(bIsParry && !DamageCauser->ActorHasTag(TEXT("Trap")))
		{
			if(bIsParryAttack == true) 
			{
				bIsParryAttack = false;
			}
			if(Damage < 40) 
			{
				Damage = 0.f;
				bIsParryAttack = true;
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitCameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);//play HitCameraShake
				UE_LOG(LogTemp, Warning, TEXT("Parry"));
			}
			else
			{
				Damage = 20.f;
				bIsParryAttack = true;
				UE_LOG(LogTemp, Warning, TEXT("Parry"));
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitCameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);//play HitCameraShake

			}
		}
		else
		{
			if(bIsBeenHit == true) 
			{
				bIsBeenHit = false;
			}
			
			bIsBeenHit = true;
			ResetAttack();
			ResetCast();
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitCameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);//play HitCameraShake
			
		}

		Health -= Damage;

		if(Health <= 0) 
		{
			bDead = true;
			AMyGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
			if(GameMode) 
			{
				GameMode->PawnKilled(this);
			}
			APlayerController* MyController = GetWorld()->GetFirstPlayerController();
			this->DisableInput(MyController);
			Camera->bUsePawnControlRotation = false;
			SetCanBeDamaged(false);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}

		UE_LOG(LogTemp, Warning, TEXT("Health left: %f"), Health);
		return Damage;
	}

	return Damage;
}

void AWizard::ParryBroken(FVector Location) 
{
	if(!bIsStunned) 
	{
		bIsStunned = true;
		ResetAttack();
		StopParry();
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		this->DisableInput(MyController);
		Camera->bUsePawnControlRotation = false;
		FVector PlayerLocation = GetActorLocation();
		FVector Direction = PlayerLocation - Location;
		//ACharacter::LaunchCharacter(Direction * 5, true, true);
	
	}
}

void AWizard::ResetStunned() 
{
	bIsStunned = false;
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	this->EnableInput(MyController);
	Camera->bUsePawnControlRotation = true;
}

void AWizard::ResetIsReceivingAttack() 
{
	bIsParryAttack = false;
}

void AWizard::ResetIsBeenHit() 
{
	bIsBeenHit = false;

}


void AWizard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	/*if(OtherActor && OtherActor->ActorHasTag(TEXT("Interactible"))) 
	{
		GhostKing = Cast<AGhostKing>(OtherActor);
		HealthPotion = Cast<AHealthPotion>(OtherActor);
		if(GhostKing) 
		{
			bIsInteractibleDialogue = true;
		}
		if(HealthPotion) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped with POTION"));
			bIsInteractiblePotion = true;
		}
	}*/
}

void AWizard::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if(OtherActor && OtherActor->ActorHasTag(TEXT("InteractibleDialogueKing"))) 
	{
		if(GhostKing && bIsDialogueSpawned) 
		{
			bIsInteractibleDialogue = false;
			GhostKing->DespawnGhostDialogue();
			bIsDialogueSpawned = false;
		}
		
	}
}

void AWizard::Interact() 
{
	/*
	if(bIsInteractibleDialogue) 
	{
		UE_LOG(LogTemp, Warning, TEXT("You Interacted with the ghost"));
		GhostKing->SpawnGhostDialogue();
		bIsDialogueSpawned = true;
	}
	if(bIsInteractiblePotion) 
	{
		if(Health < MaxHealth) 
		{
			UE_LOG(LogTemp, Warning, TEXT("You Interacted with the Potion"));
			DrinkHealthPotion();
		}
	}
	*/
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FVector Offset(0.f, 0.f, 70.f);
	FVector StartRay = GetActorLocation() + Offset;
	FVector EndRay = StartRay + Camera->GetComponentRotation().Vector() * 150.f;
	bool isHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRay, EndRay, ECollisionChannel::ECC_Visibility, QueryParams);
	if(isHit) 
	{
		if(bIsDialogueSpawned && GhostKing && HitResult.GetActor()->ActorHasTag(TEXT("InteractibleDialogueKing"))) 
		{
			GhostKing->AddTextCounter();
		}
		else 
		{
			if(HitResult.GetActor()->ActorHasTag(TEXT("InteractiblePotion"))) 
			{
				UE_LOG(LogTemp, Warning, TEXT("You Interacted"));
				DrawDebugLine(GetWorld(), StartRay, EndRay, FColor::Green, false, 3.f);
				HealthPotion = Cast<AHealthPotion>(HitResult.GetActor());
				DrinkHealthPotion();
			}

			else if(HitResult.GetActor()->ActorHasTag(TEXT("InteractibleDialogueKing"))) 
			{
				UE_LOG(LogTemp, Warning, TEXT("You Interacted"));
				GhostKing = Cast<AGhostKing>(HitResult.GetActor());
				if(!GhostKing) {return;}
				GhostKing->SpawnGhostDialogue();
				bIsDialogueSpawned = true;

			}
			else if(HitResult.GetActor()->ActorHasTag(TEXT("InteractibleBook"))) 
			{
				bIsBookMoving = true;
				UE_LOG(LogTemp, Warning, TEXT("You Interacted"));
				BookDelegate.Broadcast(bIsBookMoving);
			}
		}
		
		

	}
}

void AWizard::DrinkHealthPotion() 
{
	if(HealthPotion) 
	{
		if((Health + HealthPotionRecharge) > MaxHealth) 
		{
			Health = MaxHealth;
		}
		else 
		{
			Health += HealthPotionRecharge;
		}
		
		HealthPotion->Destroy();
	}

}





//                                                      MOVEMENT
void AWizard::MoveForward(float Value) 
{
	if(Value>0) {Forward = true;}
	if(Value<=0) {Forward = false;}
	

	AddMovementInput(GetActorForwardVector() * Value * 60.f * GetWorld()->GetDeltaSeconds());
	
	
}
	

void AWizard::MoveRight(float Value) 
{
	if(Value>0) {Right = true;}
	if(Value<0) {Left = true;}
	if(Value == 0) {Right = false, Left = false;}
	
	
	AddMovementInput(GetActorRightVector() *  Value * 60.f * GetWorld()->GetDeltaSeconds());
	
	
}





//                                                            JUMP AND LAND
void AWizard::DoubleJump() 
{
	if(MovementComp->IsFalling()) {return;}
	if(DoubleJumpCounter <=1) 
	{
		bJumping = true;
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}

}

void AWizard::Landed(const FHitResult& Hit) 
{
	MovementComp->FallingLateralFriction = 0.f;//RESET FRICTION TO 0 IF LANDED SO WHILE JUMPING THE PLAYER DOESN'T HAVE AIRCONTROL
	bJumping = false;
	DoubleJumpCounter = 0;
}





//                                                   START RUN E STOP RUN

void AWizard::StartRun() 
{
	if(!bIsParry && !MovementComp->IsFalling() && Forward) 
	{
		bIsSprinting = true;
		UE_LOG(LogTemp, Warning, TEXT("IsRunning TRUE"));
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

}

void AWizard::StopRun() 
{
	if(!bIsParry) 
	{
		bIsSprinting = false;
		UE_LOG(LogTemp, Warning, TEXT("IsRunning FALSE"));
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	

}



//                                                     DASH AND FRICTION
void AWizard::Dash() 
{
	if(bCanDash) //if did not jump twice already, execute
	{
		MovementComp->FallingLateralFriction = 4.f;//SETS A NEW FRICTION WHILE DASHING SO THAT THE PLAYER STOPS IF FALLING WHILE DASHING
		MovementComp->GroundFriction = 1.f;
		DashCounter -= 1;
		GetWorldTimerManager().SetTimer(TimerHandleForGroundFriction, this, &AWizard::ResetGroundFriction, GroundFrictionResetTime, false);
		if(Forward) {
						ACharacter::LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal(0.5f) * DashLength, true, true);
						bIsDashingForward = true;
						GetWorldTimerManager().SetTimer(TimerHandleForDashingForward, this, &AWizard::StopDashingForward, 2.f, false);
					}
		if(!Forward) {ACharacter::LaunchCharacter
				(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal(0.5f) * (-DashLength), true, true);}
		if(Right) {ACharacter::LaunchCharacter
				(FVector(GetActorRightVector().X, GetActorRightVector().Y, 0).GetSafeNormal(0.5f) * DashLength, true, true);}
		if(Left) {ACharacter::LaunchCharacter
				(FVector(GetActorRightVector().X, GetActorRightVector().Y, 0).GetSafeNormal(0.5f) * (-DashLength), true, true);}

		

		if(DashCounter <= 1) 
		{
			GetWorldTimerManager().SetTimer(TimerHandleForDash, this, &AWizard::ResetDash, 3.0f, true);//Reset Dash(Cooldown)
			if(DashCounter == 0) 
			{
				bCanDash = false;
			}
		}
	}
}

void AWizard::StopDashingForward() 
{
	bIsDashingForward = false;
	GetWorldTimerManager().ClearTimer(TimerHandleForDashingForward);
}
	

void AWizard::ResetDash() 
{
	if(DashCounter < 2) 
	{
		bCanDash = true;
		DashCounter += 1;
	}
}

void AWizard::ResetGroundFriction() 
{
	MovementComp->GroundFriction = 8.f;
	UE_LOG(LogTemp, Warning, TEXT("FRICTION RESETTATA"));
	GetWorldTimerManager().ClearTimer(TimerHandleForGroundFriction);
}


//                                                     ATTACK
void AWizard::HandAttack()//Starts checking if is charging attack
{

	if(!bIsGlowing && !bIsCharged && !bIsCharging && !bIsAttackingAnimation && !bIsAttackingNow && !bIsCasting && !bIsTelekinesisLogicStillWorking && !bIsRejected && !bIsStunned && !bIsBeenHit && !MovementComp->IsFalling() && !bIsSprinting) 
	{
		bIsCharging = true;
		GetWorldTimerManager().SetTimer(TimerHandleToCheckChargedAttack, this, &AWizard::CheckChargedAttack, 1.1f, false);
	}

	if(bIsGlowing && !bIsChargedWithOrb && !bIsChargingWithOrb && !bIsAttackingAnimation && !bIsAttackingNow && !bIsCasting && !bIsTelekinesisLogicStillWorking && !bIsRejected && !bIsStunned && !bIsBeenHit && !MovementComp->IsFalling() && !bIsSprinting) 
	{
		bIsChargingWithOrb = true;
		GetWorldTimerManager().SetTimer(TimerHandleToCheckChargedAttack, this, &AWizard::CheckChargedAttack, 0.8f, false);
	}
		
}

void AWizard::CheckChargedAttack() //Starts charged attack
{
	if(bIsCharging && !bIsGlowing) 
	{
		bIsCharged = true;
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);//Play Camera Shake
		Sword->Attack();
		UE_LOG(LogTemp, Error, TEXT("ATTACCO CARICATOOOO!!!!!"));
	}
	if(bIsChargingWithOrb && bIsGlowing) 
	{
		bIsChargedWithOrb = true;
		Sword->Attack();
		UE_LOG(LogTemp, Error, TEXT("ATTACCO CARICATOOOO CON MANAAAAAA!!!!!"));
	}

	GetWorldTimerManager().ClearTimer(TimerHandleToCheckChargedAttack);
}


void AWizard::ReleaseAttack() //Starts light Attack if is not charging(starts on release)
{
	bIsCharging = false;
	bIsChargingWithOrb = false;
	
	if(!MovementComp) {return;}
	if(!Sword) {return;}

	if(bIsParry ||bIsBeenHit) 
	{
		return;
	}

	

	if(bHasTelekinesisGrabbed) 
	{
		ResetCast();
		FVector TelekinesisLoc = TelekinesisHit->GetActorLocation();
		FVector PlayerLoc = GetActorLocation();
		FVector ImpulseVector = (TelekinesisLoc-PlayerLoc).GetSafeNormal(1000.f)*200000.f;
		TelekinesisHit->FindComponentByClass<UPrimitiveComponent>()->AddImpulse(ImpulseVector);
	}
	
	if(bIsDashingForward && !bIsChargingWithOrb && !bIsChargedWithOrb && !bIsCharged && !bIsCharging && !bIsAttackingAnimation && !bIsAttackingNow && !bIsCasting && !bIsTelekinesisLogicStillWorking && !bIsRejected && !bIsStunned && !bIsBeenHit && !bIsSprinting) 
	{
		bIsAttackingNow = true;
		bIsAttackingAnimation = true;
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);
		Sword->Attack();
		UE_LOG(LogTemp, Warning, TEXT("YOU ATTACKED, SWORD IS CHECKING"));
	}
	
	else if(!bIsChargingWithOrb && !bIsChargedWithOrb && !bIsCharged && !bIsCharging && !bIsAttackingAnimation && !bIsAttackingNow && !bIsCasting && !bIsTelekinesisLogicStillWorking && !bIsRejected && !bIsStunned && !bIsBeenHit && !MovementComp->IsFalling() && !bIsSprinting)
	{
	
		bIsAttackingNow = true;
		bIsAttackingAnimation = true;
		if(Combo==5) {Combo = -1;}
		Combo++;
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.f, 1000.f, 1.f, false);
		Sword->Attack();
		UE_LOG(LogTemp, Warning, TEXT("YOU ATTACKED, SWORD IS CHECKING"));
	}
}

//Reset the ability to attack, FUNCTION called via NOTIFY on each Attack animation
void AWizard::ResetAttack() 
{
	bIsCharging = false;
	bIsChargingWithOrb = false;
	bIsAttackingNow = false;
	bIsCharged = false;
	bIsChargedWithOrb = false;

	Sword->StopAttack();
	UE_LOG(LogTemp, Warning, TEXT("SWORD STOPPED CHECKING"));
	GetWorldTimerManager().SetTimer(TimerHandleForResetAttack, this, &AWizard::ResetAttackAnimation, 0.02f, false);

	
}

void AWizard::ResetAttackAnimation() 
{
	bIsAttackingAnimation = false;
	GetWorldTimerManager().ClearTimer(TimerHandleForResetAttack);

}






//                                                                ORB
void AWizard::PreCallOrb() 
{
	if(bIsGlowing == false && Mana>0) //Call the function that spawns the orb if F is pressed
	{
		bIsGlowing = true;
		GetWorldTimerManager().SetTimer(TimerHandleToCallOrb, this, &AWizard::CallOrb, 0.2f, false);
	}
	else if (bIsGlowing == true) 
	{
		if(NewGlowingOrb) //Destroy Orb if F is pressed again
		{
			Sword->SteelEnchant();
			NewGlowingOrb->Destroy();
			if(DespawnOrbParticle) 
			{
				FVector DespawnOrbParticleLocation = NewGlowingOrb->GetActorLocation();
				FRotator DespawnOrbParticleRotation = NewGlowingOrb->GetActorRotation();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DespawnOrbParticle, DespawnOrbParticleLocation, DespawnOrbParticleRotation);
			}
			bIsOrbTakingMana = false;
			bIsGlowing = false;

			UAnimationBlueprintLibrary::SetRateScale(Animation1, 1.8f);
			UAnimationBlueprintLibrary::SetRateScale(Animation2, 1.8f);
			UAnimationBlueprintLibrary::SetRateScale(Animation3, 1.8f);
			UAnimationBlueprintLibrary::SetRateScale(Animation4, 1.8f);
			UAnimationBlueprintLibrary::SetRateScale(Animation5, 1.8f);
			UAnimationBlueprintLibrary::SetRateScale(Animation6, 1.8f);
		}
	}
	
}

void AWizard::CallOrb() 
{
	if(BlueprintGlowingOrb && bIsGlowing) 
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbCasted"));
		bIsOrbTakingMana = true;
		Sword->ManaEnchant();
		GetWorldTimerManager().ClearTimer(TimerHandleToCallOrb);
	
		FVector Location = GetActorLocation();
		FVector Direction = GetActorForwardVector();
		FVector Direction2 = GetActorRightVector();
		FVector VectorsSub = (Direction-Direction2)*70.f;
		FVector Offset(0.f, 0.f, 120.f);
		FRotator Rotation = GetActorRotation();

		FVector FinalLocation = VectorsSub +  Location + Offset;
		NewGlowingOrb = GetWorld()->SpawnActor<AGlowingOrb>(BlueprintGlowingOrb, FinalLocation, Rotation);
		NewGlowingOrb->SetOwner(this);
		UAnimationBlueprintLibrary::SetRateScale(Animation1, 2.5f);
		UAnimationBlueprintLibrary::SetRateScale(Animation2, 2.5f);
		UAnimationBlueprintLibrary::SetRateScale(Animation3, 2.5f);
		UAnimationBlueprintLibrary::SetRateScale(Animation4, 2.5f);
		UAnimationBlueprintLibrary::SetRateScale(Animation5, 2.5f);
		UAnimationBlueprintLibrary::SetRateScale(Animation6, 2.5f);


	}
}





//                                                              SPELL

void AWizard::SetSpell() //Sets Spell
{
	if(!bIsSpellEquipped) 
	{
		bIsSpellEquipped = true;
		bIsTelekinesisEquipped = false;
		UE_LOG(LogTemp, Warning, TEXT("Spell Equipped"));
	}
}

void AWizard::SetTelekinesis() //Sets Telekinesis
{
	if(!bIsTelekinesisEquipped) 
	{
		bIsTelekinesisEquipped = true;
		bIsSpellEquipped = false;
		UE_LOG(LogTemp, Warning, TEXT("Telekinesis Equipped"));
	}
}


void AWizard::CastAttack() //Starts spell animation
{
	if(!MovementComp) {return;}
	if(!bIsCastingLogicStillWorking && bIsSpellEquipped && !bIsCasting && !bIsAttackingNow && !MovementComp->IsFalling() && !bIsSprinting && Mana >=20.f) //if there's a spell blueprint set and if all timers are clear
	{
		bIsCasting = true;
		bIsCastingLogicStillWorking = true;
	}
	else if(!bIsTelekinesis && !bIsTelekinesisLogicStillWorking && bIsTelekinesisEquipped && !bIsCasting && !bIsAttackingNow && !MovementComp->IsFalling() && !bIsSprinting && Mana >=1.f) 
	{
		bIsTelekinesis = true;
		bIsTelekinesisLogicStillWorking = true;
	}
	else if(bIsTelekinesis) 
	{
		ResetCast();
	}
	
}

void AWizard::CallCast() //Spawns spell and particle effect, FUNCTION called via NOTIFY on cast animation
{

	
	if(bIsCasting && BlueprintSpellClass) //Stuff to do with Spell
	{
		UGameplayStatics::SpawnSoundAttached(SpellCast, GetMesh(), TEXT("spellsocket"));
		UE_LOG(LogTemp, Warning, TEXT("YOU CASTED"));
		FVector SpawnLocation = SpellLocation->GetComponentLocation();
		Camera->ClearAdditiveOffset();
		FRotator SpawnRotation = Camera->GetForwardVector().Rotation();

		ASpell* NewSpell = GetWorld()->SpawnActor<ASpell>(BlueprintSpellClass, SpawnLocation, SpawnRotation);
		NewSpell->SetOwner(this);
		if(NewSpell) {Mana -= 20.f;}


	}

	else if(bIsTelekinesis) //Stuff to do with telekinesis
	{
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FVector StartRay = GetMesh()->GetSocketLocation(TEXT("spellsocket"));
		FRotator PlayerRotation = Camera->GetComponentRotation();
		FVector EndRay = StartRay+PlayerRotation.Vector()*1500.f;
		bool isHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartRay, EndRay, ECollisionChannel::ECC_PhysicsBody, QueryParams);
		if(isHit && HitResult.GetActor() != this && HitResult.GetActor()->ActorHasTag(TEXT("TelekinesisObject"))) 
		{
			bool IsPhysicsBody = HitResult.GetActor()->FindComponentByClass<UPrimitiveComponent>()->IsSimulatingPhysics();
			if(IsPhysicsBody && TelekinesisHandle) 
			{
				UE_LOG(LogTemp, Warning, TEXT("PEEEEEEEEEEEEEEW %s"), *HitResult.GetActor()->GetName());
				bHasTelekinesisGrabbed = true;
				TelekinesisHit = HitResult.GetActor();
				TelekinesisObjectDefaultMaterial = TelekinesisHit->FindComponentByClass<UMeshComponent>()->GetMaterial(1);//Store the default material inside a variable
				TelekinesisHit->FindComponentByClass<UMeshComponent>()->SetMaterial(0, TelekinesisObjectNewMaterial);//Set the new material temporarily
				
				ObjectDefaultRotation = TelekinesisHit->GetActorRotation();
				FVector ObjectLoc = TelekinesisHit->GetActorLocation();
				TelekinesisLocation->SetWorldLocation(ObjectLoc);//Sets the SceneComponent at the same location as the object held
				
				DefinitiveLocation = TelekinesisLocation->GetComponentLocation();
				DefinitiveRotation = TelekinesisLocation->GetComponentRotation();
				DefinitiveRotation.Pitch = 0.f;
				DefinitiveRotation.Roll = 0.f;
				TelekinesisHandle->SetTargetLocation(DefinitiveLocation);//Set location and rotation of PhysicsHandle equal to location and rotation of SceneComponent TelekinesisLocation
				TelekinesisHandle->SetTargetRotation(DefinitiveRotation);
				TelekinesisHandle->GrabComponentAtLocationWithRotation(TelekinesisHit->FindComponentByClass<UPrimitiveComponent>(), TEXT("None"), DefinitiveLocation, DefinitiveRotation);
				Distance = FVector::Dist(StartRay, ObjectLoc);
			}
			else 
			{
				ResetCast();
			}
		}
		else 
		{
			ResetCast();
		}
	}
	
}

//Reset the ability to cast
void AWizard::ResetCast() 
{
	bIsCasting = false;
	bIsTelekinesis = false;
	bHasTelekinesisGrabbed = false;
	if(TelekinesisHit) 
	{
		TelekinesisHit->FindComponentByClass<UMeshComponent>()->SetMaterial(0, TelekinesisObjectDefaultMaterial);
		TelekinesisHandle->ReleaseComponent();
	
	}

	GetWorldTimerManager().SetTimer(TimerHandleForResetCastLogic, this, &AWizard::ResetCastLogic, 0.01f, false);

}

void AWizard::ResetCastLogic() 
{
	bIsCastingLogicStillWorking = false;
	bIsTelekinesisLogicStillWorking = false;
	UE_LOG(LogTemp, Warning, TEXT("IT's Working"));
	GetWorldTimerManager().ClearTimer(TimerHandleForResetCastLogic);
}


void AWizard::ResetTelekinesisLogic() 
{
	bIsTelekinesisLogicStillWorking = false;
}




//                                                            PARRY
void AWizard::StartParry() 
{
	if(!bIsStunned && !bIsAttackingNow && !bIsCasting && !bIsTelekinesisLogicStillWorking && !GetMovementComponent()->IsFalling() && !bIsSprinting)
	{
		bIsParry = true;
		GetCharacterMovement()->MaxWalkSpeed = ParrySpeed;
		UE_LOG(LogTemp, Warning, TEXT("StartParry"));
	}

}

void AWizard::StopParry()
{
	if(bIsParry) 
	{
		bIsParry = false;
		bIsParryAttack = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("StopParry"));
	}
}

//                                                      GETTERS
float AWizard::GetHealth() 
{
	return Health / MaxHealth;
}

float AWizard::GetMana() 
{
	return Mana / MaxMana;
}