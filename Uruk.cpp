// Fill out your copyright notice in the Description page of Project Settings.


#include "Uruk.h"
#include "Components/StaticMeshComponent.h"
#include "UrukWeapon.h"
#include "UrukShield.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard.h"
#include "GlowingTurret.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwordManaAttack.h"

// Sets default values
AUruk::AUruk()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.f;
	PunchDamage = 10.f;
	AreaDamage = 30.f;
	bDead = false;
	bCanAttack = true;
	bIsParrying = false;
	bIsPunching = false;
	bIsBeenHit = false;
	bKick = false;
	bIsAttackingCombo = false;



}

// Called when the game starts or when spawned
void AUruk::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Weapon = GetWorld()->SpawnActor<AUrukWeapon>(BlueprintWeapon);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName ("swordsocket"));
	Weapon->SetOwner(this);

	Shield = GetWorld()->SpawnActor<AUrukShield>(BlueprintShield);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("spellsocket"));
	Shield->SetOwner(this);

	Combo = 0;
	
}

bool AUruk::GetIsBeenHit() 
{
	return bIsBeenHit;
}


void AUruk::ResetAttack() 
{
	bCanAttack = true;
	Weapon->Reset();
	GetWorldTimerManager().ClearTimer(TimerHandleForAttack);
}

void AUruk::Attack() 
{
	if(Weapon && bCanAttack && !bIsBeenHit) 
	{
		bCanAttack = false;
		Combo++;
		if(Combo>1) {Combo = 0;}
		Weapon->Attack();
		UE_LOG(LogTemp, Warning, TEXT("URUK ATTACCA!!!"));
		GetWorldTimerManager().SetTimer(TimerHandleForAttack, this, &AUruk::ResetAttack, 1.2f, false);
	}
	
}

void AUruk::AttackCombo() //Start area attack
{
	bIsAttackingCombo = true;
	
	GetWorldTimerManager().SetTimer(TimerHandleForAttackCombo, this, &AUruk::ResetAttackCombo, 3.f, false);
}

void AUruk::LaunchAttackCombo() 
{
	float Distance = CalculateDistanceToPlayer();
	if(Distance<=500.f) 
	{
		FHitResult HitResult;
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if(Player == nullptr) {return;}
		FVector StartLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,PlayerLocation,ECollisionChannel::ECC_Visibility);
		if(IsHit) 
		{
			UE_LOG(LogTemp, Error, TEXT("IS HIT!!"));
			FVector Direction = (PlayerLocation-StartLocation).GetSafeNormal();
			FVector Offset(0.f, 0.f, 150.f);
			
			
		
			Player->LaunchPawn(Offset, false, true);
			Wizard = Cast<AWizard>(Player);
			if(Wizard == nullptr) {return;}
			FPointDamageEvent DamageEvent;
			AController* ControllerUruk = this->GetInstigatorController();
			Wizard->TakeDamage(AreaDamage, DamageEvent, ControllerUruk, this);
			TimerDel2.BindUFunction(this, FName("MovePlayer"), Direction);
			GetWorld()->GetTimerManager().SetTimer(TimerHandleForPlayerMovement, TimerDel2, 0.01f, false);
		}
	}

}

void AUruk::MovePlayer(FVector Direction) 
{
	Player->LaunchPawn(Direction * 1500.f, true, true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleForPlayerMovement);
}

void AUruk::AttackPunch() //Starts Punch animation
{
	float Distance = CalculateDistanceToPlayer();
	if(!bIsBeenHit && !bIsPunching && Distance <= 150.f) 
	{ 
		bIsPunching = true;
		GetWorldTimerManager().SetTimer(TimerHandleToPunchDamage, this, &AUruk::Punch, 0.4f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Distance IS NOT OK!!"));
		GetWorldTimerManager().SetTimer(TimerHandleToPunchAttack, this, &AUruk::ResetAttackPunch, 0.4f, false);
	}
}

void AUruk::Punch() //Deals damage to player (PUNCH)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Distance between player and uruk is OK"));
	Wizard = Cast<AWizard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Wizard == nullptr) {return;}
	FPointDamageEvent DamageEvent;
	AController* ControllerUruk = this->GetInstigatorController();
	Wizard->TakeDamage(PunchDamage, DamageEvent, Controller, this);
	GetWorldTimerManager().SetTimer(TimerHandleToPunchAttack, this, &AUruk::ResetAttackPunch, 0.4f, false);
}



void AUruk::ResetAttackPunch() 
{
	bIsPunching = false;
	GetWorldTimerManager().ClearTimer(TimerHandleToPunchAttack);
}

float AUruk::CalculateDistanceToPlayer() 
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(Player) 
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector UrukLocation = GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, UrukLocation);
		return Distance;
	}
	return 0.f;
}

void AUruk::ResetAttackCombo() 
{
	bIsAttackingCombo = false;
	GetWorldTimerManager().ClearTimer(TimerHandleForAttackCombo);
}

void AUruk::Detach() 
{
	Weapon->Destroy();
	Shield->Destroy();
}

void AUruk::Parry() //FA PARTIRE IL PARRY PER UN SECONDO E MEZZO
{
	bIsParrying = true;
	Shield->ActiveParry();
	GetWorldTimerManager().SetTimer(TimerHandleToResetParry, this, &AUruk::ResetParry, 2.f, false);

}

void AUruk::ResetParry() //RESETTA IL PARRY
{
	bIsParrying = false;
	Shield->DeactiveParry();
	GetWorldTimerManager().ClearTimer(TimerHandleToResetParry);

}

// Called every frame
void AUruk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUruk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AUruk::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) 
{
	if(DamageCauser != nullptr) 
	{
		float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		UE_LOG(LogTemp, Warning, TEXT("Uruk Colpito!"))
		Damage = FMath::Min(Health, DamageAmount);
		if(bIsParrying) //SE URUK STA PARANDO, DANNO = 0
		{
			Damage = 0.f;
			bIsBeenHit = true;
			GetWorldTimerManager().SetTimer(TimerHandleForResetHit, this, &AUruk::ResetHit, 0.4f, false);
		}
		
		Health -= Damage;


		if(Health == 0) //SE LA VITA è A 0, MUORE
		{
			bDead = true;
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return Damage;
		}	

		Wizard = Cast<AWizard>(DamageCauser->GetOwner());
		Turret = Cast<AGlowingTurret>(DamageCauser->GetOwner());
		SwordMana = Cast<ASwordManaAttack>(DamageCauser);

		if(Damage > 0) 
		{
			if(Wizard && Turret == nullptr) 
			{
				UE_LOG(LogTemp, Error, TEXT("WIZARD HA SPARATO"));
				bIsBeenHit = true;
				Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				if(Player == nullptr) {return 0.f;}
				FVector PlayerLocation = Player->GetActorLocation();
				FVector UrukLocation = GetActorLocation();
				FVector Direction = UrukLocation - PlayerLocation;
				FVector CleanDirection = Direction.GetSafeNormal();
			
			//LANCIA IL CHARACTER IN ALTO, PRIMA DI LANCIARLO LUNGO L'ASSE XY
				this->LaunchCharacter(FVector(0.f, 0.f, 150.f), true, true);
			//TIMERHANDLEDELEGATE PER ASSEGNARGLI UNA FUNZIONE CON PARAMETRI
				TimerDel.BindUFunction(this, FName("MoveHit"), CleanDirection);
				GetWorld()->GetTimerManager().SetTimer(TimerHandleForHitMovement, TimerDel, 0.01f, false);
				GetWorldTimerManager().SetTimer(TimerHandleForResetHit, this, &AUruk::ResetHit, 0.5f, false);
			}
			else if(Turret)
			{
				UE_LOG(LogTemp, Error, TEXT("TURRET HA SPARATO"));
				bIsBeenHit = true;
				FVector UrukLocation = GetActorLocation();
				FVector DamageCauserLocation = Turret->GetActorLocation();
				FVector Direction = UrukLocation - DamageCauserLocation;
				FVector CleanDirection = Direction.GetSafeNormal();
				Direction.Z = 0.f;
				this->LaunchCharacter(FVector(0.f, 0.f, 150.f), true, true);//LANCIA IL CHARACTER IN ALTO, PRIMA DI LANCIARLO LUNGO L'ASSE XY
				TimerDel.BindUFunction(this, FName("MoveHit"), CleanDirection);
				GetWorld()->GetTimerManager().SetTimer(TimerHandleForHitMovement, TimerDel, 0.01f, false);
				GetWorldTimerManager().SetTimer(TimerHandleForResetHit, this, &AUruk::ResetHit, 0.5f, false);
			}
			else if(SwordMana) 
			{
				UE_LOG(LogTemp, Error, TEXT("Sword HA SPARATO"));
				bIsBeenHit = true;
				Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				if(Player == nullptr) {return 0.f;}
				FVector PlayerLocation = Player->GetActorLocation();
				FVector UrukLocation = GetActorLocation();
				FVector Direction = UrukLocation - PlayerLocation;
				FVector CleanDirection = Direction.GetSafeNormal();
			
			//LANCIA IL CHARACTER IN ALTO, PRIMA DI LANCIARLO LUNGO L'ASSE XY
				this->LaunchCharacter(FVector(0.f, 0.f, 150.f), true, true);
			//TIMERHANDLEDELEGATE PER ASSEGNARGLI UNA FUNZIONE CON PARAMETRI
				TimerDel.BindUFunction(this, FName("MoveHit"), CleanDirection);
				GetWorld()->GetTimerManager().SetTimer(TimerHandleForHitMovement, TimerDel, 0.01f, false);
				GetWorldTimerManager().SetTimer(TimerHandleForResetHit, this, &AUruk::ResetHit, 0.2f, false);
			}


		}

		return Damage;
	}
	return 0.f;
}

//CHIAMA QUESTA FUNZIONE QUANDO COLPITO, APPLICANDOGLI UNA SPINTA RELATIVA ALLA DISTANZA(VECTOR) TRA URUK(SE STESSO) E L'OGGETTO CHE LO COLPISCE
void AUruk::MoveHit(FVector CleanDirection)
{
	this->LaunchCharacter(CleanDirection * 200, true, false);
	GetWorldTimerManager().ClearTimer(TimerHandleForHitMovement);
}

void AUruk::ResetHit() 
{
	bIsBeenHit = false;
	GetWorldTimerManager().ClearTimer(TimerHandleForResetHit);
}

//CHIAMA QUESTA FUNZIONE 
void AUruk::BreakPlayerParry() //Starts uruk Kick Animation
{
	UE_LOG(LogTemp, Error, TEXT("PARRY BROKEN!!!"));
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(Player) 
	{
		Wizard = Cast<AWizard>(Player);
		if(Wizard) 
		{
			float Distance = CalculateDistanceToPlayer();
			if(Distance<=150.f) 
			{
				bKick = true;
			}
		}
		
	}
}

void AUruk::BreakParry() //Deals damage to player 
{
	float Distance = CalculateDistanceToPlayer();
	if(Distance<=150.f) 
	{
		FVector Location = GetActorLocation();
		if(Wizard) 
		{
			Wizard->ParryBroken(Location);
		}
	}
}


void AUruk::ResetKick() //Resets kick e stops animation
{
	bKick = false;
}


