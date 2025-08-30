// Unreal Engine Marketplace Copyright


#include "TitanCharacter/AOTTitanEnergyEmission.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AOTTitanBase.h"
#include "Components/WidgetComponent.h"

// Sets default values
AAOTTitanEnergyEmission::AAOTTitanEnergyEmission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	EmissionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EmissionWidget"));
	EmissionWidget->SetupAttachment(RootComponent);
	EmissionWidget->SetWidgetSpace(EWidgetSpace::World);
}

void AAOTTitanEnergyEmission::SetEnergyEmission(AAOTTitanBase* TitanBase, float EmissionRange, float EnergyPerTime)
{
	EmissionOwnerTitan = TitanBase;
	Range = EmissionRange;
	DamagePerTime = EnergyPerTime;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCapsuleRadius(Range);
	CapsuleComponent->SetCapsuleHalfHeight(Range*0.5);
	if (EmissionWidget->GetWidget())
	{
		EmissionWidget->SetBackgroundColor(FLinearColor::Red);
	}
}

void AAOTTitanEnergyEmission::ActivateEmission()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (EmissionAsset)
	{
		NiagaraComponent->SetAsset(EmissionAsset);
		NiagaraComponent->Activate();
	}
	if (EmissionWidget->GetWidget())
	{
		EmissionWidget->SetBackgroundColor(FLinearColor::Green);
	}
}

void AAOTTitanEnergyEmission::DeactivateEmission()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NiagaraComponent->Deactivate();
	if (EmissionWidget->GetWidget())
	{
		EmissionWidget->SetVisibility(false);
	}
}

void AAOTTitanEnergyEmission::HandleEmissionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// EmissionEffect
}

// Called when the game starts or when spawned
void AAOTTitanEnergyEmission::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent->SetPaused(true);
}

void AAOTTitanEnergyEmission::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AAOTTitanEnergyEmission::HandleEmissionHit);
}

void AAOTTitanEnergyEmission::FollowTarget(float DeltaTime)
{
	FHitResult HitResult;
	FVector Start = FVector(FVector2D(FollowTargetCharacter->GetActorLocation()), 800.f);
	FVector End = Start - FVector(0, 0, 10000.0f); // 아래로 충분히 긴 거리
	FVector TargetLocation = FVector::ZeroVector;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(FollowTargetCharacter); // 자기 자신 무시

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector GroundLocation = HitResult.ImpactPoint;
		TargetLocation = GroundLocation;
	}
	FVector2D CurrentLocation = FVector2D(GetActorLocation());
	FVector2D Dir = (FVector2D (TargetLocation) - CurrentLocation).GetSafeNormal();
	FVector2D NewLocation = CurrentLocation + Dir * FollowSpeed * DeltaTime * 0.01;
	FVector NewStart = FVector(NewLocation, 800.f);
	FVector NewEnd = Start - FVector(0, 0, 10000.0f); // 아래로 충분히 긴 거리
	if (GetWorld()->LineTraceSingleByChannel(HitResult, NewStart, NewEnd, ECC_Visibility, Params))
	{
		FVector GroundLocation = HitResult.ImpactPoint;
		SetActorLocation(GroundLocation);
	}
}

void AAOTTitanEnergyEmission::FollowTitanOwner(float DeltaTime)
{
	FVector Start = FVector(FVector2D(EmissionOwnerTitan->GetActorLocation()), 800.f);
	FVector End = Start - FVector(0, 0, 10000.0f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(EmissionOwnerTitan);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector GroundLocation = HitResult.ImpactPoint;
		SetActorLocation(GroundLocation);
	}
}

// Called every frame
void AAOTTitanEnergyEmission::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FColor Color = CapsuleComponent->IsCollisionEnabled() ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), GetActorLocation(), CapsuleComponent->GetUnscaledCapsuleHalfHeight(),
		CapsuleComponent->GetUnscaledCapsuleRadius(), FRotationMatrix::MakeFromX(FVector::UpVector).ToQuat(),
		Color, false, 0.1f);
	if (bFollowTarget)
	{
		FollowTarget(DeltaTime);
	}
	else if (bFollowTitan)
	{
		FollowTitanOwner(DeltaTime);
	}
}

void AAOTTitanEnergyEmission::SetFollowTarget(ACharacter* TargetCharacter, float Speed)
{
	bFollowTarget = true;
	bFollowTitan = false;
	FollowSpeed = Speed;
	FollowTargetCharacter = TargetCharacter;
	FVector TargetLocation = FollowTargetCharacter->GetActorLocation();
	FHitResult HitResult;
	FVector Start = TargetLocation;
	FVector End = Start - FVector(0, 0, 10000.0f); // 아래로 충분히 긴 거리

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(FollowTargetCharacter); // 자기 자신 무시

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector GroundLocation = HitResult.ImpactPoint;
		SetActorLocation(GroundLocation);
	}
}

void AAOTTitanEnergyEmission::SetFollowTitanOwner()
{
	bFollowTitan = true;
	bFollowTarget = false;
	FVector Start = EmissionOwnerTitan->GetActorLocation();
	FVector End = Start - FVector(0, 0, 10000.0f); // 아래로 충분히 긴 거리

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(EmissionOwnerTitan); // 자기 자신 무시
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		FVector GroundLocation = HitResult.ImpactPoint;
		SetActorLocation(GroundLocation);
	}
}

void AAOTTitanEnergyEmission::StopFollowTarget()
{
	bFollowTarget = false;
	bFollowTitan = false;
}
