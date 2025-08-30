// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AOTAT_AttackHitCheck.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

class UAOTSkillData;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTAT_AttackHitCheck : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAOTAT_AttackHitCheck();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAOTAT_AttackHitCheck* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AAOTTA_Trace> TargetActorClass, int32 _SkillNum, bool bTraceMonster = true);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	FORCEINLINE void SetSKillNum(int32 _SkillNum) { SkillNum = _SkillNum; }

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AAOTTA_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AAOTTA_Trace> SpawnedTargetActor;
	
	int32 SkillNum;

	bool bTraceMonster = true;

};
