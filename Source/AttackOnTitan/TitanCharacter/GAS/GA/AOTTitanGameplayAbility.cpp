// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"

#include "AttackOnTitan.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanHealthAttributeSet.h"


AAOTTitanBase* UAOTTitanGameplayAbility::GetTitanBaseFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AAOTTitanBase>(CurrentActorInfo->AvatarActor) : nullptr);
}

const UAOTTitanCombatAttributeSet* UAOTTitanGameplayAbility::GetCombatAttributeSet() const
{
	return GetAbilitySystemComponentFromActorInfo()->GetSet<UAOTTitanCombatAttributeSet>();
}

const UAOTTitanHealthAttributeSet* UAOTTitanGameplayAbility::GetHealthAttributeSet() const
{
	return GetAbilitySystemComponentFromActorInfo()->GetSet<UAOTTitanHealthAttributeSet>();
}

void UAOTTitanGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(GetTitanBaseFromActorInfo()->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Controller Base Is Null"));
		return;
	}
	TCB->ToggleUpdateTarget(false);
}

void UAOTTitanGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(GetTitanBaseFromActorInfo()->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Controller Base Is Null"));
		return;
	}
	TCB->ToggleUpdateTarget(true);
}

UAOTTitanGameplayAbility::UAOTTitanGameplayAbility()
{
}
