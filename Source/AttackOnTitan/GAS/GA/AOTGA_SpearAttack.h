// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_SpearAttack.generated.h"

class ACharacter;
class AAOTWeapon_Spear;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_SpearAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAOTGA_SpearAttack();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void SpearAttackMontageCallback(UAnimMontage* Montage,bool Flag);

private:
	TSubclassOf<AAOTWeapon_Spear> SpearClass;
	TObjectPtr<UAnimMontage> SpearAttackMontage;
};
