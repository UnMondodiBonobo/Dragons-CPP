// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "GhostSpell.h"
#include "Components/CapsuleComponent.h"
#include "GhostKnife.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();

	Knife = GetWorld()->SpawnActor<AGhostKnife>(BlueprintKnifeClass);
	if(!Knife) {return;}
	Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("swordsocket"));
	Knife->SetOwner(this);
	Health = MaxHealth;

	
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AGhost::TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) 
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, nullptr, DamageCauser);
	Damage = FMath::Min(Health, Damage);
	Health -= Damage;
	bIsBeenHit = true;
	UE_LOG(LogTemp, Error, TEXT("VITA RIMASTA: %f"), Health);
	
	if(Health == 0) 
	{
		bIsDead = true;
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	return Damage;
}

void AGhost::ResetHit() 
{
	bIsBeenHit = false;
}

void AGhost::StartAttack() 
{
	if(bIsAttacking == false) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ghost Attacked"));
		bIsAttacking = true;
	}
}

void AGhost::SpawnSpell() 
{
	FRotator SpawnRotation;
	FVector ViewPointLocation;
	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("spellsocket"));
	GetActorEyesViewPoint(ViewPointLocation, SpawnRotation);

		
	Spell = GetWorld()->SpawnActor<AGhostSpell>(BlueprintSpellClass, SpawnLocation, SpawnRotation);
	if(Spell)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ghost Casted"));
		Spell->SetOwner(this);
	}
}

void AGhost::StartStab() 
{
	if(bIsStabbing == false) 
	{
		 UE_LOG(LogTemp, Error, TEXT("Ghost Stabbed"));
		bIsStabbing = true;
		Knife->Stab();
	}
}

void AGhost::StopStab() 
{
	UE_LOG(LogTemp, Error, TEXT("Ghost stopped Stab"));
	bIsStabbing = false;
	Knife->StopStab();
}

void AGhost::StopAttack() 
{
	UE_LOG(LogTemp, Warning, TEXT("Ghost Stopped Attack"));
	bIsAttacking = false;
}



