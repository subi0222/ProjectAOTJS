// Unreal Engine Marketplace Copyright


#include "Player/AOTEnemyDetectComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UAOTEnemyDetectComponent::UAOTEnemyDetectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor* UAOTEnemyDetectComponent::DetectEnemy(float Range, float Radius)
{
	bDetecting = true;
	TargetDetectRadius = Radius;
	CurrentDetectRadius = 0.f;
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, GetOwner());

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * Range;

	AActor* ReturnActor = nullptr;

	bool HitDetected = GetOwner()->GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));

	if (HitDetected)
	{
		DetectEnemies.Reset();
		for (auto HitResult : OutHitResults)
		{
			DetectEnemies.Add(HitResult.GetActor());
		}
		ReturnActor = DetectEnemies[0];
		CurrentActor = ReturnActor;
		CurrentIndex = 0;
	}
	else
	{
		CurrentActor = nullptr;
		CurrentIndex = 0;
	}
	OnChangeActor.ExecuteIfBound(ReturnActor);
	return ReturnActor;
}

AActor* UAOTEnemyDetectComponent::NextEnemy()
{
	if (DetectEnemies.Num() == 0)
		return nullptr;

	CurrentIndex++;
	if (CurrentIndex >= DetectEnemies.Num())
	{
		CurrentIndex = 0;
	}
	CurrentActor = DetectEnemies[CurrentIndex];
	OnChangeActor.ExecuteIfBound(CurrentActor);
	return CurrentActor;
}

void UAOTEnemyDetectComponent::ResetDetectEnemy()
{
	CurrentIndex = -1;
	CurrentActor = nullptr;
	OnChangeActor.ExecuteIfBound(CurrentActor);
	DetectEnemies.Reset();
}


// Called when the game starts
void UAOTEnemyDetectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAOTEnemyDetectComponent::UpdateCurrentActor()
{
	if (CurrentActor == nullptr)
		return;

	for (int i =0; i<DetectEnemies.Num();i++)
	{
		AActor* Actor = DetectEnemies[i];
		if (Actor)
		{
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			FVector CurrentActorLocation = Actor->GetActorLocation();
			if (FVector::Dist(OwnerLocation, CurrentActorLocation) > 2500.f)
			{
				if (Actor == CurrentActor)
				{
					CurrentActor = nullptr;
					OnChangeActor.ExecuteIfBound(CurrentActor);
				}
				DetectEnemies.RemoveAt(i);
			}
		}
		else
			DetectEnemies.RemoveAt(i);
	}
}


// Called every frame
void UAOTEnemyDetectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(DetectEnemies.Num() >0)
		UpdateCurrentActor();
	if (CurrentActor)
	{		
		FVector Center = CurrentActor->GetActorLocation();
		DrawDebugSphere(GetWorld(), Center, 100.0f, 12, FColor::Red, false);
	}
	if (bDetecting)
	{
		if (CurrentDetectRadius >= TargetDetectRadius)
		{
			bDetecting = false;
		}
		else
		{
			CurrentDetectRadius += TargetDetectRadius * DeltaTime * 2;
			FVector Center = GetOwner()->GetActorLocation();
			DrawDebugSphere(GetWorld(), Center, CurrentDetectRadius, 12, FColor::Green, false, DeltaTime);
		}		
	}
}

