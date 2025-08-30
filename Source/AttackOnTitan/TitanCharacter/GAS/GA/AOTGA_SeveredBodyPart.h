// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_SeveredBodyPart.generated.h"

class UAOTTitanSeveredPartComponent;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_SeveredBodyPart : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	

	UFUNCTION() 
	void HandleEnd();

	void PlaySeveredMontageAndSeveredTask(const UAOTTitanSeveredPartComponent* TitanSeveredPartComp);

	void TriggerDieEvent(const UAOTTitanSeveredPartComponent* TitanSeveredPartComp);

};
