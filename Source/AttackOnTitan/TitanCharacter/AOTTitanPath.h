// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOTTitanPath.generated.h"

class USplineComponent;

UCLASS()
class ATTACKONTITAN_API AAOTTitanPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOTTitanPath();

	FVector GetPatrolPoint(int index) const;

	FVector GetNextPatrolPoint();

protected:

	UPROPERTY(EditAnywhere, Category = "Path", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	TObjectPtr<USplineComponent> SplineComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolPoints;

	UPROPERTY()
	uint8 CurrentIndex = 0;

	UPROPERTY()
	bool bIsForward;

};
