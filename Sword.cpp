// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Wizard.h"
#include "Uruk.h"
#include "UrukShield.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystem.h"
#include "SwordManaAttack.h"

class AUruk;

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(SceneComp);
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SwordMesh);

	LightAttackDamage = 10.f;
	IsRejected = false;
	IsSwordEnchanted = false;


}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	SwordMesh->SetMaterial(0, SteelMaterial);


	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlapBegin);
	
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::Attack() 
{
	if(!bCheckAttack) 
	{
		bCheckAttack = true;
		UGameplayStatics::SpawnSoundAttached(SwingSound, SwordMesh, TEXT("bladesocket"));
	}
	
	
}

void ASword::SpawnSwordMana() 
{
	FVector SwordLocation = GetActorLocation();
	AWizard* Wizard = Cast<AWizard>(GetOwner());
	if(Wizard && IsSwordEnchanted) 
	{
		FRotator SwordRotation = Wizard->Camera->GetComponentRotation();
		SwordMana = GetWorld()->SpawnActor<ASwordManaAttack>(BlueprintSwordMana, SwordLocation, SwordRotation);
		SwordMana->SetOwner(this);
	}

}

void ASword::StopAttack() 
{
	bCheckAttack = false;
}

void ASword::ManaEnchant() //Sets Mana material to swordMesh
{
	if(ManaMaterial) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Mana Material set"));
		SwordMesh->SetMaterial(0, ManaMaterial);//Changes Material
		IsSwordEnchanted = true;//Variable to check particle effect when attacking
	}
}

void ASword::SteelEnchant()  //Sets Steel Material to swordMesh
{
	if(SteelMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sword Material set"));
		SwordMesh->SetMaterial(0, SteelMaterial);//Changes Material
		IsSwordEnchanted = false;//Variable to check particle effect when attacking
	}
	
}


void ASword::Reject() 
{
	Player = Cast<AWizard>(GetOwner());
	if(Player == nullptr) {return;}
	UE_LOG(LogTemp, Error, TEXT("Reject Called"));
	IsRejected = true;
	Player->bIsRejected = true; //Cannot Attack if rejected
	Player->ResetAttack();
	GetWorldTimerManager().SetTimer(TimerHandleForReject, this, &ASword::ResetReject, 0.6f, false);
}

void ASword::ResetReject() 
{
	UE_LOG(LogTemp, Error, TEXT("Reset Reject"));
	IsRejected = false;
	Player->bIsRejected = false;//Reset attack after being rejected
	GetWorldTimerManager().ClearTimer(TimerHandleForReject);
}


void ASword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(bCheckAttack && OtherActor != nullptr && OtherActor != this && OtherActor != this->GetOwner()) 
	{
		AUruk* UrukHit = Cast<AUruk>(OtherActor);
		AGhost* GhostHit = Cast<AGhost>(OtherActor);
		if(GhostHit) 
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			if(OwnerPawn == nullptr) {return;}
						
			AController* PlayerController = OwnerPawn->GetController();
			if(PlayerController == nullptr) {return;}
						
			FVector Location;
			FRotator Rotation;
			PlayerController->GetPlayerViewPoint(Location, Rotation);
			FVector HitDirection = -Rotation.Vector();
			FPointDamageEvent DamageEvent;//FDamageEvent con costruttore vuoto, da cambiare
			GhostHit->TakeDamage(LightAttackDamage, DamageEvent, PlayerController, this);
			UE_LOG(LogTemp, Error, TEXT("GhostColpito"));
		}
		
		if(UrukHit) 
		{
			if(!UrukHit->bIsParrying)
			{
				APawn* OwnerPawn = Cast<APawn>(GetOwner());
				if(OwnerPawn == nullptr) {return;}
						
				AController* PlayerController = OwnerPawn->GetController();
				if(PlayerController == nullptr) {return;}
						
				FVector Location;
				FRotator Rotation;
				PlayerController->GetPlayerViewPoint(Location, Rotation);
				FVector HitDirection = -Rotation.Vector();
				FPointDamageEvent DamageEvent;//FDamageEvent con costruttore vuoto, da cambiare
				UrukHit->TakeDamage(LightAttackDamage, DamageEvent, PlayerController, this);

				FVector ActorHitLocation = UrukHit->GetActorLocation();
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, ActorHitLocation);//Play impact cue sound
						
				UE_LOG(LogTemp, Warning, TEXT("Attacco riuscito"));
				UE_LOG(LogTemp, Warning, TEXT("Hai attaccato : %s"), *UrukHit->GetName());	
				//bCheckAttack = false;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HAI COLPITO LO SCUDO"));
				Reject();
				bCheckAttack = false;
			}
		}
	}
}

