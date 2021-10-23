// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrapHorizontal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Wizard.h"
#include "DrawDebugHelpers.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

class AWizard;

// Sets default values
ASpikeTrapHorizontal::ASpikeTrapHorizontal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SceneComp);

	bActive = false;
	bCanDamage = true;
	Damage = 50.f;
	
}

// Called when the game starts or when spawned
void ASpikeTrapHorizontal::BeginPlay()
{
	Super::BeginPlay();
	ActivePosition = GetActorLocation().Z+200.f;
	DeactivePosition = GetActorLocation().Y;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrapHorizontal::OnOverlapBegin);
	
	GetWorldTimerManager().SetTimer(TimerHandleForTrap, this, &ASpikeTrapHorizontal::Activate, 5.f, true);

	
}

void ASpikeTrapHorizontal::ResetCooldown() 
{
	bCanDamage = true;
	GetWorldTimerManager().ClearTimer(TimerHandleForPlayerCooldown);
}

// Called every frame
void ASpikeTrapHorizontal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*if(bActive) 
	{
		FVector ActorLocation = GetActorLocation();
		float ActorLocationZ = GetActorLocation().Z;
		float UpdatedPosition = ActorLocationZ;
		UpdatedPosition = FMath::FInterpTo(UpdatedPosition, ActivePosition, DeltaTime, 9.f);
		ActorLocation.Z = UpdatedPosition;
		SetActorLocation(ActorLocation);
		

	}
	else
	{

		FVector NewActorLocation = GetActorLocation();
		float NewActorLocationY = GetActorLocation().Y;
		float NewUpdatedPosition = NewActorLocationY;
		NewUpdatedPosition = FMath::FInterpTo(NewUpdatedPosition, DeactivePosition, DeltaTime, 2.f);
		NewActorLocation.Y = NewUpdatedPosition;
		SetActorLocation(NewActorLocation);
	}*/
	
	

}

void ASpikeTrapHorizontal::Activate() 
{
	/*bActive = !bActive;

	(bActive == true) ? Animation->GetSequencePlayer()->Play() : Animation->GetSequencePlayer()->PlayReverse();*/
	

}

void ASpikeTrapHorizontal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(OtherActor == nullptr) {return;}
	//UE_LOG(LogTemp, Warning, TEXT("Trap hit OTHERACTOR %s"), *OtherActor->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Trap hit HITRESULT %s"), *Hit.GetActor()->GetName());
	
	AWizard* Giocatore = Cast<AWizard>(OtherActor);

	if(Giocatore != nullptr && bCanDamage) //If the OtherActor is AWizard type cast and check if can damage again, else do nothing
	{
		bCanDamage = false;
		UE_LOG(LogTemp, Warning, TEXT("Trap Hit something %s"), *Giocatore->GetName());
		//Giocatore->InTrappola(Damage); FUNZIONE ALTERNATIVA SOLO PER LA TRAPPOLA

		FVector Offset(0.f, 0.f, 100.f);
		FVector ShotDirection = (GetActorLocation()+Offset);
		AController* Controller = GetWorld()->GetFirstPlayerController();
		FPointDamageEvent DamageEvent(Damage, SweepResult, -ShotDirection, nullptr);
		float DamageApplied = Giocatore->TakeDamage(Damage, DamageEvent, Controller , this);

		FHitResult Hitresult;
		FCollisionQueryParams QueryParams;

		bool IsHit = GetWorld()->LineTraceSingleByChannel(Hitresult, GetActorLocation(), Giocatore->GetActorLocation(), ECollisionChannel::ECC_Visibility, QueryParams);
		if(IsHit) 
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), Giocatore->GetActorLocation(), FColor::Red, true);
		}

		FVector LocationActor = Giocatore->GetActorLocation();
		FVector LocationTrap = GetActorLocation();
		FVector Direction = LocationActor-LocationTrap;
		Direction.Z = Direction.Z/2.f;
		Giocatore->LaunchCharacter(Direction*3, true, true);
		GetWorldTimerManager().SetTimer(TimerHandleForPlayerCooldown, this, &ASpikeTrapHorizontal::ResetCooldown, 1.f, false);
	}
}


