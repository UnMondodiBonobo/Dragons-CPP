// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingTurret.h"
#include "Components/SphereComponent.h"
#include  "Uruk.h"
#include "DrawDebugHelpers.h"
#include "Spell.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AGlowingTurret::AGlowingTurret() 
{
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(Root);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(Root);


}

void AGlowingTurret::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

}

void AGlowingTurret::BeginPlay() 
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandleForShoot, this, &AGlowingTurret::CheckFireCondition, 3.f, true);

    //FVector ParticleLocation = ProjectileSpawnPoint->GetComponentLocation();
    //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, ParticleLocation);
 
}

void AGlowingTurret::CheckFireCondition() 
{
    if(bIsActive) 
    {
        TArray<AActor*> Enemies;
        SphereCollider->GetOverlappingActors(Enemies);
        for(AActor* Enemy : Enemies) 
        {
            Uruk = Cast<AUruk>(Enemy);
            if(Uruk) 
            {
                Shoot();
                break;
            }
        }
        
    }
}

void AGlowingTurret::Shoot() 
{

    if(Uruk == nullptr) {return;}
    UE_LOG(LogTemp, Error, TEXT("URUKCASTATO")); 

    FVector Location = ProjectileSpawnPoint->GetComponentLocation();
    FVector EndPoint = Uruk->GetActorLocation();
    FVector VectorDirection = EndPoint - Location;
    FRotator Direction = VectorDirection.Rotation();
    Spell = GetWorld()->SpawnActor<ASpell>(BlueprintSpell, Location, Direction);
    Spell->SetOwner(this); 

    

}
