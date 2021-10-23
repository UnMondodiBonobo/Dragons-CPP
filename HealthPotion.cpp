// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComp);

	InteractibleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractibleSphere"));
	InteractibleSphere->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

