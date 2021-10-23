// Fill out your copyright notice in the Description page of Project Settings.


#include "UrukWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard.h"
#include "Uruk.h"


// Sets default values
AUrukWeapon::AUrukWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComp);

	IsAttacking = false;
	LightAttackDamage = 15.f;

}

// Called when the game starts or when spawned
void AUrukWeapon::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
}

// Called every frame
void AUrukWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UrukOwner = Cast<AUruk>(GetOwner());

	if(IsAttacking && UrukOwner != nullptr && !UrukOwner->GetIsBeenHit())
	{
		BoxCollider->GetOverlappingActors(Enemies);
		for(AActor* Enemy : Enemies) 
		{
			AWizard* Player = Cast<AWizard>(Enemy);
			if(Player) 
			{
				UE_LOG(LogTemp, Warning, TEXT("Wizard was hit"));
				FPointDamageEvent DamageEvent;
				AController* Controller = this->GetInstigatorController();
				Player->TakeDamage(LightAttackDamage, DamageEvent, Controller, this);
				SetActorTickEnabled(false);
			}
		}

	}

}

void AUrukWeapon::Attack() 
{
	GetWorldTimerManager().SetTimer(TimerHandleForAttackDelay, this, &AUrukWeapon::StartCheckingAttack, 0.1f, false);
}

void AUrukWeapon::StartCheckingAttack() 
{
	GetWorldTimerManager().ClearTimer(TimerHandleForAttackDelay);
	UE_LOG(LogTemp, Warning, TEXT("Attack checked"));
	SetActorTickEnabled(true);
	IsAttacking = true;
}

void AUrukWeapon::Reset() 
{
	SetActorTickEnabled(false);
	IsAttacking = false;
}

