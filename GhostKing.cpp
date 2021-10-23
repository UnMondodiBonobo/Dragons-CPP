// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostKing.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AGhostKing::AGhostKing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent((SceneComp));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComp);

	InteractibleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractibleSphere"));
	InteractibleSphere->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void AGhostKing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostKing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGhostKing::SpawnGhostDialogue() 
{
	if(BlueprintGhostDialogue && !bIsDialogueAlreadySpawned) 
	{
		GhostDialogue = CreateWidget(GetWorld(), BlueprintGhostDialogue);
		if(GhostDialogue) 
		{
			GhostDialogue->AddToViewport();
			bIsDialogueAlreadySpawned = true;
		}
	}
}

void AGhostKing::DespawnGhostDialogue() 
{
	if(BlueprintGhostDialogue) 
	{
		if(GhostDialogue) 
		{
			GhostDialogue->Destruct();
			bIsDialogueAlreadySpawned = false;
			TextCounter = 0;
		}
	}
}

void AGhostKing::AddTextCounter() 
{
	if(TextCounter > 0) {return;}
	TextCounter++;
}


