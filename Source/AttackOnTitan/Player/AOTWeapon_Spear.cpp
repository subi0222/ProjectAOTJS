// Unreal Engine Marketplace Copyright


#include "Player/AOTWeapon_Spear.h"
#include "AttackOnTitan/Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AAOTWeapon_Spear::AAOTWeapon_Spear()
{
    PrimaryActorTick.bCanEverTick = true;

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = Capsule;
    Capsule->SetCapsuleSize(42.f,96.f);
    Capsule->SetCollisionProfileName("Spear");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));   

    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->Velocity = GetActorUpVector() * ProjectileMovement->InitialSpeed;
   //ProjectileMovement->bRotationFollowsVelocity = true; // 진행 방향으로 회전
    ProjectileMovement->bIsHomingProjectile = true; // 유도 모드 활성화
   
}

void AAOTWeapon_Spear::BeginPlay()
{
    Super::BeginPlay();    

   ProjectileMovement->HomingAccelerationMagnitude = 40000.f; // 유도 강도 설정
   Capsule->OnComponentBeginOverlap.AddDynamic(this, &AAOTWeapon_Spear::OnBeginOverlap);
   if (TargetActor)
   {
       ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
   }
}

void AAOTWeapon_Spear::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetActor)
    {
        FVector SpearLocation = GetActorLocation();
        FVector TargetLocation = TargetActor->GetActorLocation();
        FVector DiffLocation = TargetLocation - SpearLocation;

        FRotator NewRotation = DiffLocation.Rotation();
        NewRotation.Pitch -= 90.f;
        SetActorRotation(NewRotation);
    }
}



void AAOTWeapon_Spear::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionParticle, GetActorLocation(), GetActorRotation());
        OnOverlapDelegate.ExecuteIfBound(TargetActor);
        Destroy();
    }
}
