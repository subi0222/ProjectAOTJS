// Unreal Engine Marketplace Copyright


#include "SkillActor/AOTProjectileSkillActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Data/AOTSkillActorData.h"
#include "Data/AOTProjectileSkillActorData.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Enum.h"

AAOTProjectileSkillActor::AAOTProjectileSkillActor()
{
}



void AAOTProjectileSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (_SkillActorData->Particle)
		UGameplayStatics::SpawnEmitterAttached(_SkillActorData->Particle, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale, true);

	if (_SkillActorData->NiagaraParticle)
		UNiagaraFunctionLibrary::SpawnSystemAttached(_SkillActorData->NiagaraParticle, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale, true);

	UAOTProjectileSkillActorData* SkillActorData = Cast<UAOTProjectileSkillActorData>(_SkillActorData);
	if (SkillActorData)
	{
		if (SkillActorData->_Shape == SkillActorShape::Box)
		{
			BoxComp->SetBoxExtent(SkillActorData->CollisionSize);
			BoxComp->SetCollisionProfileName(_SkillActorData->CollisionName);
			BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AAOTProjectileSkillActor::OnOverlapBeginProjectile);
		}
		else if (SkillActorData->_Shape == SkillActorShape::Capsule)
		{
			CapsuleComp->SetCapsuleSize(SkillActorData->CollisionSize.Z,SkillActorData->CollisionSize.X);
			CapsuleComp->SetCollisionProfileName(_SkillActorData->CollisionName);
		}
	}

	ProjectileComp->InitialSpeed = SkillActorData->Speed;
	
	FVector RotatedDir;
	FVector ForwardDir = GetOwner()->GetActorRotation().Vector();
	if (SkillActorData->bDirFront)
		RotatedDir = ForwardDir.RotateAngleAxis(SkillActorData->DirOffset,FVector::UpVector);
	else
	{
		FVector RightVector = GetOwner()->GetActorRightVector();
		RotatedDir = ForwardDir.RotateAngleAxis(-SkillActorData->DirOffset,RightVector);
	}
	RotatedDir.Normalize();
	ProjectileComp->Velocity = RotatedDir * SkillActorData->Speed;
	
	if (_TargetActor != nullptr)
	{
		ProjectileComp->bIsHomingProjectile = true;
		ProjectileComp->HomingAccelerationMagnitude = 40000.f;
		ProjectileComp->HomingTargetComponent = _TargetActor->GetRootComponent();
	}
	
}

void AAOTProjectileSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetActorLocation().Z >= 0.f)
		ProjectileComp->Velocity = FVector::ZeroVector;
}

void AAOTProjectileSkillActor::OnOverlapBeginProjectile(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAOTProjectileSkillActorData* SkillActorData = Cast<UAOTProjectileSkillActorData>(_SkillActorData);
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		for (auto& EffectClass : SkillActorData->TargetEffects)
		{
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			Context.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, Context);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	if (SkillActorData->bTarget)
		Destroy();
}
