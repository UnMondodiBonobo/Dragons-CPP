// Fill out your copyright notice in the Description page of Project Settings.

#include "SpikeTrap.h"
#include "Wizard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

class AWizard;


// Sets default values
ASpikeTrap::ASpikeTrap()
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
	TrapActiveVelocity = 10.f;

	
}

// Called when the game starts or when spawned
void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	ActivePosition = GetActorLocation().Z+200.f;
	DeactivePosition = GetActorLocation().Z;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnOverlapBegin);
	
	GetWorldTimerManager().SetTimer(TimerHandleForTrap, this, &ASpikeTrap::Activate, TrapTiming, true);

	
}


// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(bActive) 
	{
		FVector ActorLocation = GetActorLocation();
		float ActorLocationZ = GetActorLocation().Z;
		float UpdatedPosition = ActorLocationZ;
		UpdatedPosition = FMath::FInterpTo(UpdatedPosition, ActivePosition, DeltaTime, TrapActiveVelocity);
		ActorLocation.Z = UpdatedPosition;
		SetActorLocation(ActorLocation);
		

	}
	else
	{

		FVector NewActorLocation = GetActorLocation();
		float NewActorLocationZ = GetActorLocation().Z;
		float NewUpdatedPosition = NewActorLocationZ;
		NewUpdatedPosition = FMath::FInterpTo(NewUpdatedPosition, DeactivePosition, DeltaTime, 2.f);
		NewActorLocation.Z = NewUpdatedPosition;
		SetActorLocation(NewActorLocation);
	}
	
	

}

void ASpikeTrap::Activate() 
{
	bActive = !bActive;
	

}

void ASpikeTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
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
		DrawDebugLine(GetWorld(), GetActorLocation(), Direction, FColor::Red, true);
		Direction.Z = Direction.Z/2.f;
		Giocatore->LaunchCharacter(Direction*3, true, true);
		GetWorldTimerManager().SetTimer(TimerHandleForPlayerCooldown, this, &ASpikeTrap::ResetCooldown, 1.f, false);
	}
}

void ASpikeTrap::ResetCooldown() 
{
	bCanDamage = true;
	GetWorldTimerManager().ClearTimer(TimerHandleForPlayerCooldown);
}


