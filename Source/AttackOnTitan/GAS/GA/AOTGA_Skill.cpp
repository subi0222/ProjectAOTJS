// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_Skill.h"
#include "Character/BaseCharacter.h"
#include "MotionWarpingComponent.h"
#include "Data/AOTSkillData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/AOTPlayerHUD.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "UI/AOTUW_PlayerBar.h"

UAOTGA_Skill::UAOTGA_Skill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = Cast<ABaseCharacter>(ActorInfo->AvatarActor);
	if (Character)
	{
		SkillData = Character->GetSkillData(SkillNum);

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (SkillData->bWarping)
			{
				UMotionWarpingComponent* MWC = Character->GetMotionWarpingComponent();
				float WarpDistance = SkillData->WarpDistance;
				FRotator NewRotation = Character->GetActorRotation();
				FVector ForwardVec = Character->GetActorForwardVector();
				FVector NewLocation = Character->GetActorLocation() + ForwardVec * WarpDistance;
				MWC->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Jumping"), NewLocation, NewRotation);
			}
			else if(!SkillData->bMoving)
				Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			if (SkillData->BuffEffectClass)
			{
				FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(SkillData->BuffEffectClass);
				if (EffectHandle.IsValid())
					ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectHandle);
			}

			AController* OwnerController = Character->GetController();
			AAOTPlayerHUD* HUD = Cast<AAOTPlayerHUD>(Character->GetHUD());
			if (HUD)
				HUD->PlayerBar->SetSkillCoolTime(SkillNum, SkillData->CoolTime);
			
			UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Skill"), SkillData->SkillMontage);
			Task->OnCompleted.AddDynamic(this, &UAOTGA_Skill::OnMontageCompleteCallback);
			Task->ReadyForActivation();
		}
	}
}

void UAOTGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	SkillData = nullptr;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);	
}

void UAOTGA_Skill::OnMontageCompleteCallback()
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
