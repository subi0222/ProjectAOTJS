// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TitanDeadData.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UTitanDeadData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	uint8 GetDeadSectionCount() const { return DeadSectionCount; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	TObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	uint8 DeadSectionCount;
	
};
