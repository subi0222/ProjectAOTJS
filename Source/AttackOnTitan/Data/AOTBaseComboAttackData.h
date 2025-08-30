// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AOTBaseComboAttackData.generated.h"

USTRUCT(BlueprintType)
struct FComboInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float AttackRange;
	UPROPERTY(EditAnywhere)
	float AttackRadius;
	UPROPERTY(EditAnywhere)
	float AttackDamage;
	UPROPERTY(EditAnywhere)
	float CanInputTime;
	UPROPERTY(EditAnywhere)
	float FailInputTime;
};

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTBaseComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Prefix)
	FString Prefix;
	UPROPERTY(EditAnywhere, Category = Combo)
	int32 ComboCount;
	UPROPERTY(EditAnywhere,Category = Combo)
	TArray<FComboInfo> ComboInfo;
};
