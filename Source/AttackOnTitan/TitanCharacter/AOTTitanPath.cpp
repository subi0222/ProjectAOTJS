// Unreal Engine Marketplace Copyright


#include "AOTTitanPath.h"

#include "Components/SplineComponent.h"

AAOTTitanPath::AAOTTitanPath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
}

FVector AAOTTitanPath::GetPatrolPoint(int index) const
{
	if (index >= SplineComp->GetNumberOfSplinePoints())
	{
		return SplineComp->GetWorldLocationAtSplinePoint(0);
	}
	return SplineComp->GetWorldLocationAtSplinePoint(index);
}

FVector AAOTTitanPath::GetNextPatrolPoint()
{
	if (SplineComp->GetNumberOfSplinePoints() == 0)
	{
		return FVector3d::Zero();
	}
	
	if (bIsForward)
	{
		if (CurrentIndex + 1 >= SplineComp->GetNumberOfSplinePoints())
		{
			bIsForward = false;
			return SplineComp->GetWorldLocationAtSplinePoint(--CurrentIndex);
		}
		return SplineComp->GetWorldLocationAtSplinePoint(++CurrentIndex);
	}

	//뒤로 가야함.
	if (CurrentIndex - 1 < 0)
	{
		bIsForward = true;
		return SplineComp->GetWorldLocationAtSplinePoint(++CurrentIndex);
	}
	return SplineComp->GetWorldLocationAtSplinePoint(--CurrentIndex);
}
