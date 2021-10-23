// Fill out your copyright notice in the Description page of Project Settings.


#include "UrukShield.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Sword.h"
#include "Spell.h"

// Sets default values
AUrukShield::AUrukShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComp);

	bIsParry = false;


}

// Called when the game starts or when spawned
void AUrukShield::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AUrukShield::OnOverlapBegin);
	
}

// Called every frame
void AUrukShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUrukShield::ActiveParry() 
{
	bIsParry = true;
}

void AUrukShield::DeactiveParry() 
{
	bIsParry = false;
}

void AUrukShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(OtherActor && bIsParry && OtherActor != this && OtherActor != GetOwner()) 
	{
		ASword* PlayerSword = Cast<ASword>(OtherActor);
		//ASpell* PlayerSpell = Cast<ASpell>(OtherActor);

		if(PlayerSword) 
		{
			UE_LOG(LogTemp, Warning, TEXT("SWORD HITS SHIELD!!"));
		}
	}
		
}


