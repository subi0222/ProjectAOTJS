// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_ModeChange.generated.h"

class ACharacter;
class AAOTWeapon_Spear;
class USkeletalMeshComponent;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_ModeChange : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAOTGA_ModeChange();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void ChangeModeMontageCallback(UAnimMontage* Montage, bool Flag);
	
private:
	bool DualBladeMode;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> RightWeaponMesh;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> LeftWeaponMesh;
	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> RightSpear;
	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> LeftSpear;
	UPROPERTY()
	TObjectPtr<UAnimMontage> ModeChangeMontage;
};
