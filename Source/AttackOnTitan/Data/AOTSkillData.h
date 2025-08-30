// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AOTSkillData.generated.h"

class UGameplayEffect;
class AAOTTA_Trace;
class UAnimMontage;
class UAOTSkillActorData;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTSkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 SkillNum;
	UPROPERTY(EditAnywhere)
	float SkillRange;
	UPROPERTY(EditAnywhere)
	float SkillRadius;
	UPROPERTY(EditAnywhere)
	float ZOffset;
	UPROPERTY(EditAnywhere)
	float ForwardOffset;
	UPROPERTY(EditAnywhere)
	float RotateOffset;
	UPROPERTY(EditAnywhere)
	float CoolTime;
	UPROPERTY(EditAnywhere)
	uint8 bMoving : 1;
	UPROPERTY(EditAnywhere)
	uint8 bSweep : 1;
	UPROPERTY(EditAnywhere)
	uint8 bWarping : 1;
	UPROPERTY(EditAnywhere)
	float WarpDistance;
	UPROPERTY(EditAnywhere)
	float DecalDuration;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAOTSkillActorData> SkillActorData;
	UPROPERTY(EditAnywhere)
	TArray<FVector> SkillActorOffsets;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> SkillEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> BuffEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAOTTA_Trace> TargetActorClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> PreMontage;
};
