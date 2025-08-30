// Unreal Engine Marketplace Copyright


#include "GAS/AT/AOTAT_AttackHitCheck.h"
#include "GAS/TA/AOTTA_Trace.h"
#include "AbilitySystemComponent.h"

UAOTAT_AttackHitCheck::UAOTAT_AttackHitCheck()
{
}

UAOTAT_AttackHitCheck* UAOTAT_AttackHitCheck::CreateTask(UGameplayAbility* OwningAbility,
                                                         TSubclassOf<class AAOTTA_Trace> TargetActorClass, int32 _SkillNum, bool bTraceMonster)
{
	UAOTAT_AttackHitCheck* NewTask = NewAbilityTask<UAOTAT_AttackHitCheck>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	NewTask->SetSKillNum(_SkillNum);
	NewTask->bTraceMonster = bTraceMonster;
	return NewTask;
}

void UAOTAT_AttackHitCheck::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAOTAT_AttackHitCheck::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UAOTAT_AttackHitCheck::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AAOTTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAOTAT_AttackHitCheck::OnTargetDataReadyCallback);
		SpawnedTargetActor->SetSkillNum(SkillNum);
		SpawnedTargetActor->SetTraceMonster(bTraceMonster);
	}
}

void UAOTAT_AttackHitCheck::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UAOTAT_AttackHitCheck::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
