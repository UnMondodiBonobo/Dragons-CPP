// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostSpell.h"
#include "Wizard.h"
#include "Ghost.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGhostSpell::AGhostSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileSphereComp"));
	SetRootComponent(ProjectileSphereComp);

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("CastRoot"));
	SceneComp->SetupAttachment(ProjectileSphereComp);

	ProjectileParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticleSystem"));
	ProjectileParticleSystem->SetupAttachment(ProjectileSphereComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SpellLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	SpellLight->SetupAttachment(ProjectileSphereComp);

	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->Friction = 0.f;

	SpellDamage = 20.f;

}

// Called when the game starts or when spawned
void AGhostSpell::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSphereComp->OnComponentHit.AddDynamic(this, &AGhostSpell::OnHit);
	
}

// Called every frame
void AGhostSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GhostOwner = Cast<AGhost>(GetOwner());
	if(GhostOwner) 
	{
		FRotator Rotation = GhostOwner->GetActorRotation();
		SetActorRotation(Rotation);
		SetActorTickEnabled(false);
	}
	
}

void AGhostSpell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit) 
{

	if(!OtherActor) {return;}


	UE_LOG(LogTemp, Warning, TEXT("CAST hit something"));
	if(GhostOwner == nullptr) 
	{
		SpellLight->SetIntensity(7000.f);
		SpellLight->SetAttenuationRadius(400.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DespawnParticle, HitComp->GetComponentLocation(), HitComp->GetComponentRotation());
		Destroy();
		return;
	}
	
	Controller = GhostOwner->GetController();
	if(Controller == nullptr) {return;}
	FVector ShotDirection = GhostOwner->GetActorLocation().GetSafeNormal();

	//bool IsSimulating = OtherActor->FindComponentByClass<UPrimitiveComponent>()->IsSimulatingPhysics();
	Wizard = Cast<AWizard>(OtherActor);
	if(Wizard) 
	{
		FPointDamageEvent DamageEvent;
		Wizard->TakeDamage(SpellDamage, DamageEvent, Controller, this);
		

	}

	if(OtherActor->ActorHasTag(TEXT("DestructibleBarrel"))) 
	{
		//FAI DISTRUGGERE I BARREL APPLICANDO UN IMPULSO
		
	}
	
	if(DespawnParticle) 
	{
		SpellLight->SetIntensity(7000.f);
		SpellLight->SetAttenuationRadius(400.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DespawnParticle, HitComp->GetComponentLocation(), HitComp->GetComponentRotation());
		Destroy();
	}

	
}

