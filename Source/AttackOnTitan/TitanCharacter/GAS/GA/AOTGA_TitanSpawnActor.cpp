// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_TitanSpawnActor.h"

#include "AttackOnTitan.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "TitanCharacter/Components/AOTTitanCombatStateComponent.h"
#include "TitanCharacter/GAS/AT/AOTAT_TitanSpawnActorTask.h"

void UAOTGA_TitanSpawnActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AAOTTitanBase* TitanBase = GetTitanBaseFromActorInfo();
	if (!TitanBase)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Base Is null"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UAOTTitanCombatStateComponent* CombatStateComp =
		TitanBase->GetComponentByClass<UAOTTitanCombatStateComponent>();
	if (!CombatStateComp)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("CombatState Comp Is null"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	TArray<FSpawnActorInformation> SpawnActorInformations = CombatStateComp->GetCurrentAttackSpawnActorInformations();
	if (SpawnActorInformations.Num() == 0)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("There is no SpawnActorInformation"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(TitanBase->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Controller Base Is Null"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	//TO DO : Weapon도 구현할 경우!!!
	UAOTAT_TitanSpawnActorTask* SpawnTask =
		UAOTAT_TitanSpawnActorTask::CreateSpawnActorTask(this, SpawnActorInformations, TCB->GetTargetCharacter());
	SpawnTask->ReadyForActivation();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
