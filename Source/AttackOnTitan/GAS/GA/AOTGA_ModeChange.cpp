// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_ModeChange.h"
#include "Character/BaseCharacter.h"
#include "Player/AOTWeapon_Spear.h"
#include "GameFramework/CharacterMovementComponent.h"

UAOTGA_ModeChange::UAOTGA_ModeChange()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_ModeChange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Player = Cast<ABaseCharacter>(ActorInfo->AvatarActor.Get());
	if (Player)
	{
		RightWeaponMesh = Player->GetRightWeaponSkeletalMesh();
		LeftWeaponMesh = Player->GetLeftWeaponSkeletalMesh();
		RightSpear = Player->GetRightSpear();
		LeftSpear = Player->GetLeftSpear();
		ModeChangeMontage = Player->GetModeChangeMontage();

		if (Player->ModeType == Mode::DualBlade)
		{
			RightWeaponMesh->SetSkeletalMesh(nullptr);
			LeftWeaponMesh->SetSkeletalMesh(nullptr);
		}
		else if(Player->ModeType == Mode::Spear)
		{
			RightSpear->SetLifeSpan(0.01f);
			LeftSpear->SetLifeSpan(0.01f);
		}
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(ModeChangeMontage);
		FOnMontageEnded EndChangeModeDelegate;
		EndChangeModeDelegate.BindUObject(this, &UAOTGA_ModeChange::ChangeModeMontageCallback);
		Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndChangeModeDelegate);
	}
	else
		EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
}

void UAOTGA_ModeChange::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAOTGA_ModeChange::ChangeModeMontageCallback(UAnimMontage* Montage, bool Flag)
{
	CurrentActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(EMovementMode::MOVE_Walking);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
