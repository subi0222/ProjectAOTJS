// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_TitanDie.h"

#include "AttackOnTitan.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TitanCharacter/Components/AOTTitanHealthComponent.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"
#include "TitanCharacter/GAS/AT/AOTAT_SeveredBodyPartTask.h"

void UAOTGA_TitanDie::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	//SeveredPartComp와 HealthComp가 Trigger할 수 있음.
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const UAOTTitanHealthComponent* HealthComponent =
		Cast<UAOTTitanHealthComponent>(TriggerEventData->OptionalObject);

	//만약 SeveredPartComp에서 Trigger 했으면 OptionalObject2에 SeveredPartComp가 있고 거기에 있는 몽타주를 플레이 해야 함 
	const UAOTTitanSeveredPartComponent* SeveredPartComp =
		Cast<UAOTTitanSeveredPartComponent>(TriggerEventData->OptionalObject2);

	UAnimMontage* PlayMontage = nullptr;

	if (SeveredPartComp)
	{
		PlayMontage = SeveredPartComp->GetRandomSeveredMontage();
	}

	if (HealthComponent)
	{
		PlayMontage = HealthComponent->GetRandomDieMontage();
	}

	if (!PlayMontage)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Montage To Play Is Nullptr"))
		if (SeveredPartComp)
		{
			UAOTAT_SeveredBodyPartTask* SeveredTask =
				UAOTAT_SeveredBodyPartTask::CreateSeveredBodyPartTask(this, SeveredPartComp);
			SeveredTask->ReadyForActivation();
		}
		HandleEnd();
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, PlayMontage);

	MontageTask->OnBlendOut.AddDynamic(this, &UAOTGA_TitanDie::HandleEnd);
	MontageTask->OnCancelled.AddDynamic(this, &UAOTGA_TitanDie::HandleEnd);

	if (SeveredPartComp)
	{
		UAOTAT_SeveredBodyPartTask* SeveredTask =
			UAOTAT_SeveredBodyPartTask::CreateSeveredBodyPartTask(this, SeveredPartComp);
		MontageTask->OnBlendOut.AddDynamic(SeveredTask, &UAOTAT_SeveredBodyPartTask::ReadyForActivation);
		HealthComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UAOTTitanHealthComponent>();
		if (HealthComponent)
		{
			HealthComponent->OnTitanDieStartDelegate.Broadcast();
		}
	}

	MontageTask->ReadyForActivation();
}

// void UAOTGA_TitanDie::PlayDieMontage(UAnimMontage* Montage)
// {
// 	//몽타주 없으면 바로 죽도록
// 	if (!Montage)
// 	{
// 		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("DieMontage Is Null"));
// 		HandleEnd();
// 		return;
// 	}
// 	UAbilityTask_PlayMontageAndWait* MontageTask =
// 		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage);
// 	MontageTask->OnBlendOut.AddDynamic(this, &UAOTGA_TitanDie::HandleEnd);
// 	MontageTask->OnCancelled.AddDynamic(this, &UAOTGA_TitanDie::HandleEnd);
// 	MontageTask->ReadyForActivation();
// }

void UAOTGA_TitanDie::HandleEnd()
{
	//HealthComp가 없다면 바로 EndAbility하고 끝냄
	UAOTTitanHealthComponent* HealthComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<
		UAOTTitanHealthComponent>();
	if (!HealthComponent)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("HealthComp Is Null"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	//Ragdoll상태로 만들고 끝낼 것
	HealthComponent->SetDead(true);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
