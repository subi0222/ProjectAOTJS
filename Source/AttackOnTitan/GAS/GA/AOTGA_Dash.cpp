// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/BaseCharacter.h"
#include "AOTGameplayTag.h"

UAOTGA_Dash::UAOTGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	if (USkeletalMeshComponent* MeshComp = ActorInfo->AvatarActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		OwnerAnimInstance = MeshComp->GetAnimInstance();
	}

	if (OwnerAnimInstance)
	{
		UCharacterMovementComponent* MovementComp = ActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
		if (MovementComp->IsFalling())
			OwnerAnimInstance->Montage_Play(AirDashMontage);
		else
			OwnerAnimInstance->Montage_Play(DashMontage);
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUObject(this, &UAOTGA_Dash::DashMontageEndCallback);
		OwnerAnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	}
	else
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UAOTGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAOTGA_Dash::DashMontageEndCallback(UAnimMontage* Montage, bool Flag)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
