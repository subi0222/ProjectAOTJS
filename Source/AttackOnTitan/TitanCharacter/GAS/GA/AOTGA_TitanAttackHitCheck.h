// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_TitanAttackHitCheck.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_TitanAttackHitCheck : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

protected:

	void ApplyDamageEffect(ABaseCharacter* HitCharacter);

protected:

	UPROPERTY(EditAnywhere, Category = HitGameplayEffect)
	TSubclassOf<UGameplayEffect> HitEffectClass;

};
