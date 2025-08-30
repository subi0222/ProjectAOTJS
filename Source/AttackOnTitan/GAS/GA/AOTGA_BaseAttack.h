// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_BaseAttack.generated.h"

class UAOTBaseComboAttackData;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_BaseAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAOTGA_BaseAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();
	void NextCombo();
	void CanInputTimeCallback();
	void FailInputTimeCallback();
	FName GetNextSector();

private:
	int32 CurrentCombo;
	bool CanNextInput = false;
	FTimerHandle CanInputTimerHandle;
	FTimerHandle FailInputTimerHandle;
	UPROPERTY()
	TObjectPtr<UAOTBaseComboAttackData> CurrentComboData;
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

};
