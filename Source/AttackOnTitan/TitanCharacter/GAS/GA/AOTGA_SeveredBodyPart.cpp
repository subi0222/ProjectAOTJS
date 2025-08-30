// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_SeveredBodyPart.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"
#include "TitanCharacter/GAS/AT/AOTAT_SeveredBodyPartTask.h"

void UAOTGA_SeveredBodyPart::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	const UAOTTitanSeveredPartComponent*
		SeveredPartComp = Cast<UAOTTitanSeveredPartComponent>(TriggerEventData->OptionalObject);
	if (!SeveredPartComp)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("OptionalObject Is Not SeveredPartComp"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	//만약 약점 포인트라면 죽도록 해야 함
	if (SeveredPartComp->IsWeaknessPart())
	{
		TriggerDieEvent(SeveredPartComp);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	PlaySeveredMontageAndSeveredTask(SeveredPartComp);
}

void UAOTGA_SeveredBodyPart::HandleEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UAOTGA_SeveredBodyPart::TriggerDieEvent(const UAOTTitanSeveredPartComponent* TitanSeveredPartComp)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("ASC Is Null"))
		return;
	}
	
	FGameplayEventData EventData;
	EventData.OptionalObject2 = TitanSeveredPartComp;
	EventData.EventTag = Titan_Event_StartDying;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActorFromActorInfo(),
		Titan_Event_StartDying,
		EventData);
}

void UAOTGA_SeveredBodyPart::PlaySeveredMontageAndSeveredTask(const UAOTTitanSeveredPartComponent* TitanSeveredPartComp)
{
	if (!TitanSeveredPartComp)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("SeveredPartComp Is Null"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAOTAT_SeveredBodyPartTask* SeveredTask =
		UAOTAT_SeveredBodyPartTask::CreateSeveredBodyPartTask(this, TitanSeveredPartComp);
	
	UAnimMontage* SeveredMontage = TitanSeveredPartComp->GetRandomSeveredMontage();
	if (!SeveredMontage)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("SeveredMontage Is Null"));
		SeveredTask->ReadyForActivation();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SeveredMontage);
	
	MontageTask->OnBlendOut.AddDynamic(this, &UAOTGA_SeveredBodyPart::HandleEnd);
	MontageTask->OnBlendOut.AddDynamic(SeveredTask, &UAOTAT_SeveredBodyPartTask::ReadyForActivation);
	MontageTask->ReadyForActivation();
}