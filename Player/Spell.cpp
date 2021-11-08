// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ghost.h"
#include "Uruk.h"

class AUruk;

// Sets default values
ASpell::ASpell()
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

	ProjectileSphereComp->SetNotifyRigidBodyCollision(true);


}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSphereComp->OnComponentHit.AddDynamic(this, &ASpell::OnHit);

	
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit) 
{

	AActor* MyOwner = GetOwner();
	if(!MyOwner) {return;}
	if(!OtherActor) {return;}


	UE_LOG(LogTemp, Warning, TEXT("CAST hit something"));
	OwnerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(OwnerPawn == nullptr) {return;}
	PlayerController = OwnerPawn->GetController();
	if(PlayerController == nullptr) {return;}
	FVector ShotDirection = OwnerPawn->GetActorLocation().GetSafeNormal();

	//bool IsSimulating = OtherActor->FindComponentByClass<UPrimitiveComponent>()->IsSimulatingPhysics();
	AUruk* Uruk = Cast<AUruk>(OtherActor);
	AGhost* Ghost = Cast<AGhost>(OtherActor);

	if (Ghost) 
	{
		FDamageEvent DamageEvent;
		Ghost->TakeDamage(SpellDamage, DamageEvent, PlayerController, this);
	}
	if(Uruk) 
	{
		FPointDamageEvent DamageEvent;
		Uruk->TakeDamage(SpellDamage, DamageEvent, PlayerController, this);
		

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





	