// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordManaAttack.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sword.h"
#include "Kismet/GameplayStatics.h"
#include "Uruk.h"

// Sets default values
ASwordManaAttack::ASwordManaAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComp);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SwordManaDamage = 10.f;

}

// Called when the game starts or when spawned
void ASwordManaAttack::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASwordManaAttack::OnOverlapBegin);
	
	
}


void ASwordManaAttack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(OtherActor != this) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), GetActorRotation());
	}
	AUruk* Uruk = Cast<AUruk>(OtherActor);
	if(Uruk) 
	{
		APawn* OwnerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if(OwnerPawn == nullptr) {return;}
		AController* PlayerController = OwnerPawn->GetController();
		if(PlayerController == nullptr) {return;}
		UE_LOG(LogTemp, Error, TEXT("COLPITOOOOO!!!"));
		FVector ShotDirection = OwnerPawn->GetActorLocation();
		FVector ShotLocation = Uruk->GetActorLocation();
		FVector FinalDirection = ShotLocation - ShotDirection;
		FPointDamageEvent DamageEvent;
		Uruk->TakeDamage(SwordManaDamage, DamageEvent, PlayerController, this);;
		//Destroy();
	}
}

// Called every frame
void ASwordManaAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ASword* OwnerSword = Cast<ASword>(GetOwner());
	if(OwnerSword) 
	{
		FRotator SwordRotation = OwnerSword->GetActorRotation();
		SetActorRotation(SwordRotation);
		SetActorTickEnabled(false);
	}

}

