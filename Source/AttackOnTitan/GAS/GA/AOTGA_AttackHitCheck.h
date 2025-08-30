// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_AttackHitCheck.generated.h"

class UAOTSkillData;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UAOTGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	float CurrentLevel;

	UPROPERTY()
	TObjectPtr<UAOTSkillData> SkillData;	

	UPROPERTY(EditAnywhere)
	uint8 IsPlayer : 1;

};
