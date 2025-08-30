// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/Data/AOTTitanAttackData.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_TitanAttack.generated.h"

/**
 * 
 */
class UAOTTitanBodyPartComponent;

//class UAnimMontage;
UCLASS()
class ATTACKONTITAN_API UAOTGA_TitanAttack : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()

public:

	UAOTGA_TitanAttack();

	// void SetAttackInformation(const FAttackInformation AttackInformation);
	
	// virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	FGameplayTagContainer GetTriggerTags() const;

private:

	UFUNCTION()
	void HandleEnd();

private:

	UPROPERTY(EditAnywhere, Category = AttackInformation, meta = (AllowPrivateAccess = true))
	FAttackInformation AbilityAttackInformation;
	
};
