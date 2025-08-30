// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_AttackHitCheck.h"
#include "GAS/AT/AOTAT_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AttackOnTitan.h"
#include "GAS/AttributeSet/AOTPlayerAttributeSet.h"
#include "Data/AOTSkillData.h"
#include "GAS/AOTPlayerAbilitySystemComponent.h"
#include "Perception/AISense_Damage.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"

UAOTGA_AttackHitCheck::UAOTGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	int32 SkillNum = TriggerEventData->EventMagnitude;
	UAOTPlayerAbilitySystemComponent* SourceASC = Cast<UAOTPlayerAbilitySystemComponent>(
		GetAbilitySystemComponentFromActorInfo_Checked());
	SkillData = SourceASC->GetSkillData(SkillNum);


	UAOTAT_AttackHitCheck* AttackTraceTask = UAOTAT_AttackHitCheck::CreateTask(
		this, SkillData->TargetActorClass, SkillNum, IsPlayer);
	AttackTraceTask->OnComplete.AddDynamic(this, &UAOTGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UAOTGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		FGameplayEffectContextHandle EffectContext;
		EffectContext.AddHitResult(HitResult);
		FGameplayEffectSpecHandle EffectHandle = GetAbilitySystemComponentFromActorInfo()->
													MakeOutgoingSpec(SkillData->SkillEffectClass, 1.0f, EffectContext);
		if (EffectHandle.IsValid())
		{
			UAISense_Damage::ReportDamageEvent(HitResult.GetActor()->GetWorld(), HitResult.GetActor(), GetAvatarActorFromActorInfo(), 1.0f, HitResult.Location, HitResult.Location);
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectHandle, TargetDataHandle);
			UAOTTitanSeveredPartComponent* ServeredPartComp =Cast<UAOTTitanSeveredPartComponent>(HitResult.GetComponent());
			if (ServeredPartComp)
			{
				AOT_LOG(LogAOTGAS, Log, TEXT("%s"), TEXT("Got Hit!"));
				ServeredPartComp->GotHit(10000);
			}
		}
	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(
			SkillData->SkillEffectClass, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
			                                EffectSpecHandle, TargetDataHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
