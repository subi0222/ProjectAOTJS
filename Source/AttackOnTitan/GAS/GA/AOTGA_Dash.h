// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_Dash.generated.h"

class UAOTGameEffect;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_Dash : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UAOTGA_Dash();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void DashMontageEndCallback(UAnimMontage* Montage, bool Flag);

private:
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> DashMontage;
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> AirDashMontage;
	UPROPERTY()
	TObjectPtr<UAnimInstance> OwnerAnimInstance;
};
