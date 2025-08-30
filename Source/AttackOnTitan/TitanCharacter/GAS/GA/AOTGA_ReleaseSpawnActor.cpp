// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_ReleaseSpawnActor.h"

#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "TitanCharacter/Interface/AOTTitanRangedAttackActorInterface.h"

void UAOTGA_ReleaseSpawnActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AAOTTitanBase* TitanBase = GetTitanBaseFromActorInfo();
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(TitanBase->GetController());
	if (TitanBase && TCB)
	{
		ACharacter* Target = TCB->GetTargetCharacter();
		TArray<AActor*> SpawnActors = TitanBase->GetSpawnActors();
		for (AActor* SpawnActor : SpawnActors)
		{
			if (IAOTTitanRangedAttackActorInterface* RangedAttackActorInterfaceInterface = Cast<IAOTTitanRangedAttackActorInterface>(SpawnActor))
			{
				RangedAttackActorInterfaceInterface->FollowTarget(Target);
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
