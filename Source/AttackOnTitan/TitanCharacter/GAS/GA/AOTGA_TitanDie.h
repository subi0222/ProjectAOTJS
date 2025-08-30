// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_TitanDie.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_TitanDie : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void HandleEnd();

	// void PlayDieMontage (UAnimMontage* Montage);

	
};
