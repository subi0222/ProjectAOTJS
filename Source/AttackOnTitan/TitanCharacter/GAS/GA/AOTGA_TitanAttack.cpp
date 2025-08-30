// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_TitanAttack.h"

#include "AbilitySystemComponent.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "TitanCharacter/Components/AOTTitanCombatStateComponent.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"

UAOTGA_TitanAttack::UAOTGA_TitanAttack()
{
}

void UAOTGA_TitanAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAOTTitanCombatStateComponent* CombatStateComp = GetTitanBaseFromActorInfo()->GetComponentByClass<UAOTTitanCombatStateComponent>();
	if (!CombatStateComp)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("CombatStateComp Is Null"))
		return;
	}

	UAnimMontage* AttackMontage = CombatStateComp->GetCurrentAttackInformation().AttackMontage;
	if (!AttackMontage)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Attack Montage is Null"));
		HandleEnd();
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None,AttackMontage);

	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleEnd);
	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleEnd);
	MontageTask->ReadyForActivation();

	AAOTTitanControllerBase* TCB =
		Cast<AAOTTitanControllerBase>(GetTitanBaseFromActorInfo()->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("TitanControllerBase Is Null"));
		return;
	}
	TCB->SetAttacking(true);
}

void UAOTGA_TitanAttack::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	AAOTTitanBase* TitanBase = GetTitanBaseFromActorInfo();
	if (!TitanBase)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("TitanBase is Null"));
		return;
	}
	UAOTTitanCombatStateComponent* CombatStateComponent = TitanBase->FindComponentByClass<UAOTTitanCombatStateComponent>();
	if (!CombatStateComponent)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Base Does Not Have CombatStateComponent"));
		return;
	}
	
	//잘못되었다면 Stamina는 -1을 반환
	float RequiredStamina = CombatStateComponent->GetRequiredStamina();
	
	if (RequiredStamina <= 0)
	{
		return;
	}

	//정상적으로 필요한 Stamina를 알고 있을 때
	else
	{
		if (!CostGameplayEffectClass)
		{
			AOT_LOG(LogAOTGAS, Log, TEXT("%s"), TEXT("CostGameplayEffectClass가 할당되지 않았습니다"));
			return;
		}
		FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(CostGameplayEffectClass, GetAbilityLevel());

		// SetByCaller 값 설정
		CostSpecHandle.Data->SetSetByCallerMagnitude(
			Titan_Effect_Stamina, RequiredStamina
		);

		// 자원 소모 적용
		UAbilitySystemComponent* TitanASC = GetAbilitySystemComponentFromActorInfo();

		if (TitanASC && CostSpecHandle.IsValid())
		{
			TitanASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
		}
	}
	
}

void UAOTGA_TitanAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AAOTTitanBase* TitanBase = GetTitanBaseFromActorInfo();
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanBase Is Nullptr"));
		return;
	}
	
	TitanBase->NotifyAttackEnd();
	
	UAOTTitanCombatStateComponent* TitanCombatStateComp = GetTitanBaseFromActorInfo()->GetComponentByClass<UAOTTitanCombatStateComponent>();
	if (!TitanCombatStateComp)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("CombatStateComp Is Nullptr"));
		return;
	}

	AAOTTitanControllerBase* TCB =
		Cast<AAOTTitanControllerBase>(GetTitanBaseFromActorInfo()->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("TitanControllerBase Is Null"));
		return;
	}
	TCB->SetAttacking(false);
	
	// TitanCombatStateComp
}

void UAOTGA_TitanAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	GetTitanBaseFromActorInfo()->NotifyAttackEnd();
}

FGameplayTagContainer UAOTGA_TitanAttack::GetTriggerTags() const
{
	FGameplayTagContainer OTriggerTagsContainer;
	for (FAbilityTriggerData TriggerData :AbilityTriggers)
	{
		OTriggerTagsContainer.AddTag(TriggerData.TriggerTag);
	}
	return OTriggerTagsContainer;
}

void UAOTGA_TitanAttack::HandleEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
