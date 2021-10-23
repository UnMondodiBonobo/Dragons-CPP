// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingPlatform.h"
#include "GlowingOrb.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/Set.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard.h"

// Sets default values
AGlowingPlatform::AGlowingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Root);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(Root);

	bIsActive = false;
	

}

// Called when the game starts or when spawned
void AGlowingPlatform::BeginPlay()
{
	Super::BeginPlay();
	//this->SetActorHiddenInGame(true);
	Mesh->SetCollisionProfileName(TEXT("PlatformDeactive"));
	BoxCollider->SetCollisionProfileName(TEXT("PlatformDeactive"));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGlowingPlatform::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AGlowingPlatform::OnOverlapEnd);
	
}

void AGlowingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(!OtherComp) {return;}
	if(OverlappedComp == BoxCollider) 
	{
		if(OtherComp->ComponentHasTag(TEXT("OrbSphere")) && !bIsAlreadyOverlapping) 
		{
			bIsActive = true;
			bIsAlreadyOverlapping = true;
			//this->SetActorHiddenInGame(false);
			Mesh->SetCollisionProfileName(TEXT("PlatformActive"));
			BoxCollider->SetCollisionProfileName(TEXT("PlatformActive"));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());
		}
		
	}
}

void AGlowingPlatform::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if(!OtherComp) {return;}
	if(OverlappedComp == BoxCollider) 
	{
		if(OtherComp->ComponentHasTag(TEXT("OrbSphere")) && bIsAlreadyOverlapping) 
		{
			bIsActive = false;
			bIsAlreadyOverlapping = false;
			//this->SetActorHiddenInGame(true);
			Mesh->SetCollisionProfileName(TEXT("PlatformDeactive"));
			BoxCollider->SetCollisionProfileName(TEXT("PlatformDeactive"));
		}
		
	}
}

// Called every frame
void AGlowingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

