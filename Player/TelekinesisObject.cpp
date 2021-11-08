// Fill out your copyright notice in the Description page of Project Settings.


#include "TelekinesisObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Wizard.h"

// Sets default values
ATelekinesisObject::ATelekinesisObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void ATelekinesisObject::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATelekinesisObject::OnOverlapBegin);
	
}

void ATelekinesisObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(OtherActor != this) 
	{
		AWizard* Player = Cast<AWizard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if(Player) 
		{
			Player->ResetCast();
		}
	}
}

// Called every frame
void ATelekinesisObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

