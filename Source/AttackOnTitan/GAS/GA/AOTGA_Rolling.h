// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_Rolling.generated.h"

class ACharacter;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_Rolling : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAOTGA_Rolling();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION()
	void RollMontageCallback();

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> AnimMontageRoll;
};
