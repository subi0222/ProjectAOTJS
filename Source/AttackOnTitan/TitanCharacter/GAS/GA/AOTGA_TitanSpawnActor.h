// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_TitanSpawnActor.generated.h"


struct FSpawnActorInformation;
class AAOTTitanEnergyEmission;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_TitanSpawnActor : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};