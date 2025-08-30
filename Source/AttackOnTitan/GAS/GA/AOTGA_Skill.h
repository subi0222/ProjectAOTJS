// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_Skill.generated.h"

class UAOTSkillData;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_Skill : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UAOTGA_Skill();
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    
    protected:
    UFUNCTION()
    virtual void OnMontageCompleteCallback();
    
    protected:
    	UPROPERTY()
    	TObjectPtr<UAOTSkillData> SkillData;
    	UPROPERTY(EditAnywhere)
    	int32 SkillNum;
};
