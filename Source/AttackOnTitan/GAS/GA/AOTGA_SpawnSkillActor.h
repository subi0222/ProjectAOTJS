// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_SpawnSkillActor.generated.h"

class AAOTSkillActor;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_SpawnSkillActor : public UGameplayAbility
{
	GENERATED_BODY()
	public:
    	UAOTGA_SpawnSkillActor();
    	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    	
    private:
    	UPROPERTY()
    	TObjectPtr<AAOTSkillActor> SpawnedSkillActor;
};
