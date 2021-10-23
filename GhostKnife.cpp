// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostKnife.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Wizard.h"

// Sets default values
AGhostKnife::AGhostKnife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp= CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnifeMesh"));
	KnifeMesh->SetupAttachment(SceneComp);

	KnifeBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("KnifeBoxCollider"));
	KnifeBoxCollider->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void AGhostKnife::BeginPlay()
{
	Super::BeginPlay();

	KnifeBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGhostKnife::OnOverlapBegin);
	
}

void AGhostKnife::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AGhost* GhostOwner = Cast<AGhost>(GetOwner());
	if(!GhostOwner) {return;}

	if(bIsKnifeStabbing && OtherActor && OtherActor != this) 
	{
		AWizard* Wizard = Cast<AWizard>(OtherActor);
		if(Wizard) 
		{
			FDamageEvent DamageEvent;
			UE_LOG(LogTemp, Error, TEXT("DANNEGGIATO!!!"));
			Wizard->TakeDamage(KnifeLightDamage, DamageEvent, this->GetInstigatorController(), this);
			bIsKnifeStabbing = false;
		}
	}
}

// Called every frame
void AGhostKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGhostKnife::Stab() 
{
	bIsKnifeStabbing= true;
}

void AGhostKnife::StopStab() 
{
	bIsKnifeStabbing = false;
}

