// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeTrap.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Wizard.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABladeTrap::ABladeTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	RotationalBlade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotationalBlade"));
	RotationalBlade->SetupAttachment(SceneComp);

	MeshForRotation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshForRotation"));
	MeshForRotation->SetupAttachment(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RotationalBlade);

	Thruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Thruster"));
	Thruster->SetupAttachment(RotationalBlade);

	Thruster2 = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Thruster2"));
	Thruster2->SetupAttachment(RotationalBlade);

	Damage = 40.f;
	bCanDamage = true;



}

// Called when the game starts or when spawned
void ABladeTrap::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABladeTrap::OnOverlapBegin);	
}

void ABladeTrap::Swing() 
{
	SetActorTickEnabled(true);
	GetWorldTimerManager().ClearTimer(TimerHandleToStartSwing);
	
}

void ABladeTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
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

		bool IsHit = GetWorld()->LineTraceSingleByChannel(Hitresult, BoxCollider->GetComponentLocation(), Giocatore->GetActorLocation(), ECollisionChannel::ECC_Visibility, QueryParams);
		if(IsHit) 
		{
			DrawDebugLine(GetWorld(), BoxCollider->GetComponentLocation(), Giocatore->GetActorLocation(), FColor::Red, true);
		}

		FVector LocationActor = Giocatore->GetActorLocation();
		FVector LocationTrap = BoxCollider->GetComponentLocation();
		FVector Direction = LocationActor-LocationTrap;
		Direction.Z = Direction.Z/2.f;
		Giocatore->LaunchCharacter(Direction*3, true, true);
		GetWorldTimerManager().SetTimer(TimerHandleForPlayerCooldown, this, &ABladeTrap::ResetCooldown, 1.f, false);
	}
}

void ABladeTrap::ResetCooldown() 
{
	bCanDamage = true;
	GetWorldTimerManager().ClearTimer(TimerHandleForPlayerCooldown);
}

// Called every frame
void ABladeTrap::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	FRotator ActualBladeRotation = RotationalBlade->GetComponentRotation();

	
	if(ActualBladeRotation.Pitch >85.f) 
	{
		
		FVector Location = GetActorLocation();
		FVector ToLeft(0.f, 0.f, 100.f);
		FVector Offset = Location + ToLeft;
		RotationalBlade->AddImpulse((Location-Offset)*2000.f);
		GetWorldTimerManager().SetTimer(TimerHandleToStartSwing, this, &ABladeTrap::Swing, 0.2f, false);
		SetActorTickEnabled(false);
	}
	else if(ActualBladeRotation.Pitch < -85.f) 
	{
	
		FVector Location = GetActorLocation();
		FVector ToLeft(0.f, 0.f, 100.f);
		FVector Offset = Location + ToLeft;
		RotationalBlade->AddImpulse((Location-Offset)*2000.f);
		GetWorldTimerManager().SetTimer(TimerHandleToStartSwing, this, &ABladeTrap::Swing, 0.2f, false);
		SetActorTickEnabled(false);
	}

}

