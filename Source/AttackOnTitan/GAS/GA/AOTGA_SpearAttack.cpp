// Unreal Engine Marketplace Copyright


#include "AOTGA_SpearAttack.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Player/AOTWeapon_Spear.h"

UAOTGA_SpearAttack::UAOTGA_SpearAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_SpearAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Player = Cast<ABaseCharacter>(ActorInfo->AvatarActor.Get());
	if (Player)
	{
		SpearClass = Player->GetSpearClass();
		SpearAttackMontage = Player->GetSpearAttackMontage();
		UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
		if (SpearClass && SpearAttackMontage && AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UAOTGA_SpearAttack::SpearAttackMontageCallback);
			AnimInstance->Montage_Play(SpearAttackMontage, 1.0f);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, SpearAttackMontage);
		}
		else
			EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
	}
}

void UAOTGA_SpearAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAOTGA_SpearAttack::SpearAttackMontageCallback(UAnimMontage* Montage, bool Flag)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
