// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "AOTTitanGameplayAbility.generated.h"


class AAOTTitanBase;
/**
 * 
 */
class UAOTTitanAbilitySystemComponent;

UCLASS()
class ATTACKONTITAN_API UAOTTitanGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UAOTTitanGameplayAbility();

	// ----------------------------------------------------------------------------------------------------------------
//
//	The important functions:
//	
//		CanActivateAbility()	- const function to see if ability is activatable. Callable by UI etc
//
//		TryActivateAbility()	- Attempts to activate the ability. Calls CanActivateAbility(). Input events can call this directly.
//								- Also handles instancing-per-execution logic and replication/prediction calls.
//		
//		CallActivateAbility()	- Protected, non virtual function. Does some boilerplate 'pre activate' stuff, then calls ActivateAbility()
//
//		ActivateAbility()		- What the abilities *does*. This is what child classes want to override.
//	
//		CommitAbility()			- Commits reources/cooldowns etc. ActivateAbility() must call this!
//		
//		CancelAbility()			- Interrupts the ability (from an outside source).
//
//		EndAbility()			- The ability has ended. This is intended to be called by the ability to end itself.
//	
// ----------------------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Titan|Ability")
	AAOTTitanBase* GetTitanBaseFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Titan|CombatAttributeSet")
	const UAOTTitanCombatAttributeSet* GetCombatAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Titan|HealthAttributeSet")
	const UAOTTitanHealthAttributeSet* GetHealthAttributeSet() const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
};
