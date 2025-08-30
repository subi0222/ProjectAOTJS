// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_Rolling.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AttackOnTitan/Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAOTGA_Rolling::UAOTGA_Rolling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_Rolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Player = Cast<ABaseCharacter>(ActorInfo->AvatarActor.Get());
	if (Player)
	{
		if (Player->GetCharacterMovement()->IsFalling())
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		AnimMontageRoll = Player->GetRollMontage();
		if (AnimMontageRoll)
		{
			UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Skill"), AnimMontageRoll);
			Task->OnCompleted.AddDynamic(this, &UAOTGA_Rolling::RollMontageCallback);
			Task->ReadyForActivation();
		}
	}		
}

void UAOTGA_Rolling::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAOTGA_Rolling::RollMontageCallback()
{
	AnimMontageRoll = nullptr;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
