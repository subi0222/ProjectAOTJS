// Unreal Engine Marketplace Copyright


#include "TitanCharacter/AOTTitanProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AOTTitanBase.h"
#include "AttackOnTitan.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "Character/GrapplingHookCharacter.h"

// Sets default values
AAOTTitanProjectile::AAOTTitanProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	ProjectileMeshComponent->SetupAttachment(SphereComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAOTTitanProjectile::FollowTarget(ACharacter* Character)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (!Character)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Target Is Nullptr"))
		Destroy();
		return;
	}
	SetActorTickEnabled(true);
	ProjectileMovementComponent->SetActive(true);
	SphereComponent->SetSimulatePhysics(false);
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bAutoActivate = true;
	SetActorEnableCollision(true);
	ProjectileMovementComponent->Activate(true);
	ProjectileMovementComponent->MaxSpeed = 10000000000.85;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
	ProjectileMovementComponent->InitialSpeed = Speed;
	FVector Direction = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Speed * Direction;
	SphereComponent->OnComponentHit.AddDynamic(this, &AAOTTitanProjectile::HandleNormalThrowProjectileHit);
}

bool AAOTTitanProjectile::HitCheck()
{
	return true;
}


// void AAOTTitanProjectile::RollGoToTarget(ACharacter* TargetCharacter, float Time, float Damage)
// {
// 	this->HitDamage = Damage;
//
// 	ProjectileMovementComponent->SetActive(false);
// 	SphereComponent->SetSimulatePhysics(true);
// 	SphereComponent->SetEnableGravity(true);
// 	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 	ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
// 	SetActorEnableCollision(true);
// 	FVector ProjectileLocation = GetActorLocation();
// 	FVector2D PlayerLocation = FVector2D(TargetCharacter->GetActorLocation());
// 	float CalculatedSpeed = CalculateSpeed(TargetCharacter, Time) * 15;
//
// 	FVector Direction = (FVector(PlayerLocation.X, PlayerLocation.Y, 0.0f) - ProjectileLocation).GetSafeNormal();
// 	ProjectileMeshComponent->AddImpulse( CalculatedSpeed * Direction);
//
// 	DrawDebugLine(GetWorld(),ProjectileLocation, ProjectileLocation + Direction * 200, FColor::Red, false, 1);
//
// 	FVector AngularImpulse = FVector(500.f, 0.f, 0.f); // X축 기준 회전
// 	ProjectileMeshComponent->AddAngularImpulseInDegrees(AngularImpulse, NAME_None, true);
//
// 	// 충돌 처리 바인딩
// 	SphereComponent->OnComponentHit.AddDynamic(this, &AAOTTitanProjectile::HandleRollThrowProjectileHit);
// 	SetLifeSpan(1.6f);
// }


// void AAOTTitanProjectile::RandomGoToTarget(ACharacter* TargetCharacter, float Time, float Damage)
// {
// 	this->HitDamage = Damage;
// 	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
// 	ProjectileMovementComponent->SetActive(true);
// 	SphereComponent->SetSimulatePhysics(false);
// 	ProjectileMeshComponent->SetSimulatePhysics(false);
// 	ProjectileMovementComponent->bShouldBounce = false;
// 	ProjectileMovementComponent->bRotationFollowsVelocity = true;
// 	ProjectileMovementComponent->bAutoActivate = true;
// 	ProjectileMovementComponent->ProjectileGravityScale = FMath::RandRange(0.8f, 1.3f);
// 	SetActorEnableCollision(true);
// 	ProjectileMovementComponent->Activate(true);
// 	ProjectileMovementComponent->MaxSpeed = 100000;
// 	float CalculatedSpeed = CalculateSpeed(TargetCharacter, Time) * 15;
// 	float RandomSpeed = FMath::RandRange(CalculatedSpeed * 0.6f, CalculatedSpeed * 1.2f);
// 	FVector BaseDirection = (TargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
//
// 	// 분산 범위 설정 (도 단위)
// 	float MaxAngleOffsetDeg = 10.0f; // 예: ±10도
//
// 	// 회전 오프셋 적용
// 	FRotator RandomRot = FRotator(
// 		FMath::FRandRange(-MaxAngleOffsetDeg, MaxAngleOffsetDeg), // Pitch
// 		FMath::FRandRange(-MaxAngleOffsetDeg, MaxAngleOffsetDeg), // Yaw
// 		0.0f
// 	);
//
// 	FVector ScatteredDirection = RandomRot.RotateVector(BaseDirection);
// 	ProjectileMovementComponent->Velocity = RandomSpeed * RandomRot.RotateVector(BaseDirection);
// 	SphereComponent->OnComponentHit.AddDynamic(this, &AAOTTitanProjectile::HandleNormalThrowProjectileHit);
// }

// Called when the game starts or when spawned
void AAOTTitanProjectile::BeginPlay()
{
	Super::BeginPlay();
	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(GetOwner());
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanBase Is Nullptr"));
		return;
	}
	OnProjectileDestroy.BindUObject(TitanBase, &AAOTTitanBase::RemoveSpawnActor);
	SetActorTickEnabled(false);
}

void AAOTTitanProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProjectileMeshComponent->AddLocalRotation(FRotator(1.0f, 0.f, 0.f) * Torque * DeltaTime);
}

void AAOTTitanProjectile::HandleNormalThrowProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                                         const FHitResult& Hit)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC)
	{
		for (auto ProjectileEffectClass  : ProjectileEffectClasses)
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpecHandle  = ASC->MakeOutgoingSpec(ProjectileEffectClass,
				1,
				EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(Player_Effect_Damage, HitDamage);
				ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			}
		}
	}
	OnProjectileDestroy.Execute(this);
	Destroy();
}

// void AAOTTitanProjectile::HandleRollThrowProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	if (AGrapplingHookCharacter* Character = Cast<AGrapplingHookCharacter>(OtherActor))
// 	{
// 		FDamageEvent DamageEvent;
// 		Character->TakeDamage(HitDamage, DamageEvent, nullptr, this);
// 		Destroy();
// 		return;
// 	}
// 	if (GetVelocity().Size() < KINDA_SMALL_NUMBER)
// 	{
// 		Destroy();
// 	}
// }

// float AAOTTitanProjectile::CalculateSpeed(ACharacter* TargetCharactger, float Time)
// {
// 	double Distance = FVector::Distance(GetActorLocation(), TargetCharactger->GetActorLocation());
// 	return Distance / Time;
// }
