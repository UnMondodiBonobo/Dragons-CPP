// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingOrb.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGlowingOrb::AGlowingOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	DespawnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DespawnParticle"));
	DespawnParticle->SetupAttachment(SceneComp);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComp);

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	Light->SetupAttachment(SceneComp);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void AGlowingOrb::BeginPlay()
{
	Super::BeginPlay();
	
		//PlayerPawn reference to make despawn
		if(SpawnParticle) 
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			APawn* PlayerPawn = PC->GetPawn();

			FVector OffsetParticle(0.f, 0.f, 120.f);
			FVector LocationParticle = PlayerPawn->GetActorLocation();
			FVector DirectionParticle = PlayerPawn->GetActorForwardVector();
			FVector DirectionParticle2 = PlayerPawn->GetActorRightVector();
			FVector FinalDirectionParticle = (DirectionParticle-DirectionParticle2)*90.f;
			FRotator Rotation = PlayerPawn->GetActorRotation();
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticle, FinalDirectionParticle+LocationParticle+OffsetParticle, Rotation);

		}
}

// Called every frame
void AGlowingOrb::Tick(float DeltaTime)
{
	//Move Position of Orb at the Exact position of the Player+Offset
	Super::Tick(DeltaTime);
	FVector Offset(0.f, 0.f, 120.f);
	FVector Location = GetOwner()->GetActorLocation();
	FVector Direction = GetOwner()->GetActorForwardVector();
	FVector Direction2 = GetOwner()->GetActorRightVector();
	FVector FinalDirection = (Direction-Direction2)*70.f;

	SetActorLocation(FinalDirection +  Location + Offset, true);
	
}

