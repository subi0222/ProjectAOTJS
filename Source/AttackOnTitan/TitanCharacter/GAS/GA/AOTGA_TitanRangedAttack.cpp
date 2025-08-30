// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_TitanRangedAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "TitanCharacter/AOTTitanProjectile.h"
// #include "TitanCharacter/TitanBase.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"


void UAOTGA_TitanRangedAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	// {
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// 	return;
	// }
	// TargetCharacter = GetTitanBaseFromActorInfo()->GetTargetCharacter();
	// UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ThrowMontage);
	// MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleCancelled);
	// MontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleEnd);
	// MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleEnd); 
	//
	// UAbilityTask_WaitGameplayEvent* GrabTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Titan_Event_RangedAttack_GrabProjectile, nullptr, true);
	// GrabTask->EventReceived.AddDynamic(this, &ThisClass::HandleProjectileGrabbed);
	// GrabTask->ReadyForActivation();
	//
	// UAbilityTask_WaitGameplayEvent* SmashTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Titan_Event_RangedAttack_SmashProjectile, nullptr, true);
	// SmashTask->EventReceived.AddDynamic(this, &ThisClass::UAOTGA_TitanRangedAttack::HandleProjectileSmashed);
	// SmashTask->ReadyForActivation();
	//
	// UAbilityTask_WaitGameplayEvent* ThrowEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Titan_Event_RangedAttack_ThrowProjectile, nullptr, true);
	// BindEventTask(ThrowEventTask);
	//
	// MontageTask->Activate();

}

void UAOTGA_TitanRangedAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAOTGA_TitanRangedAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAOTGA_TitanRangedAttack::HandleCancelled()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
}

void UAOTGA_TitanRangedAttack::HandleEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UAOTGA_TitanRangedAttack::HandleProjectileGrabbed(FGameplayEventData Payload)
{
	if (AAOTTitanProjectile* TitanProjectile = GetWorld()->SpawnActor<AAOTTitanProjectile>(ProjectileClass, FTransform::Identity))
	{
		FTransform SpawnTransform = FTransform::Identity;
		// TitanProjectile->AttachToComponent(GetTitanBaseFromActorInfo()->GetMesh(),
			// FAttachmentTransformRules::SnapToTargetIncludingScale,
			// AttatchSocketName);
		Projectiles.Add(TitanProjectile);
	}
}

void UAOTGA_TitanRangedAttack::HandleProjectileNormalThrown(FGameplayEventData Payload)
{
	check(Projectiles.Num() == 1);
	for (AAOTTitanProjectile* TitanProjectile : Projectiles)
	{
		TitanProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		check(TargetCharacter);
		// TitanProjectile->NormalGoToTarget(TargetCharacter, ThrowTime, GetCombatAttributeSet()->GetSpecialAttackDamage());
	}
}

void UAOTGA_TitanRangedAttack::HandleProjectileScatterThrown(FGameplayEventData Payload)
{
	check(Projectiles.Num() > 1)
	for (AAOTTitanProjectile* TitanProjectile : Projectiles)
	{
		if (TitanProjectile)
		{
			TitanProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			// TitanProjectile->RandomGoToTarget(TargetCharacter, ThrowTime, GetCombatAttributeSet()->GetSpecialAttackDamage() / ProjectileNum);
		}
	}
}

void UAOTGA_TitanRangedAttack::HandleProjectileRollThrown(FGameplayEventData Payload)
{
	for (AAOTTitanProjectile* TitanProjectile : Projectiles)
	{
		TitanProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		check(TargetCharacter);
		// TitanProjectile->RollGoToTarget(TargetCharacter, ThrowTime, GetCombatAttributeSet()->GetSpecialAttackDamage());
	}
}

void UAOTGA_TitanRangedAttack::BindEventTask(UAbilityTask_WaitGameplayEvent* EventTask)
{
	switch (RangedAttackType)
	{
	case ERangedAttackType::RollThrow:
		EventTask->EventReceived.AddDynamic(this, &ThisClass::HandleProjectileRollThrown);
		break;

	case ERangedAttackType::NormalThrow:
		EventTask->EventReceived.AddDynamic(this, &ThisClass::HandleProjectileNormalThrown);
		break;

	case ERangedAttackType::ScatterThrow:
		EventTask->EventReceived.AddDynamic(this, &ThisClass::HandleProjectileScatterThrown);
		break;

	default:
		break;
	}
	EventTask->ReadyForActivation();
}

void UAOTGA_TitanRangedAttack::HandleProjectileSmashed(FGameplayEventData Payload)
{
	// check(Projectiles.Num() == 1);
	// for (auto TitanProjectile : Projectiles)
	// 	TitanProjectile->Destroy();
	// for (int i = 0; i < ProjectileNum; i++)
	// {
	// 	if (ATitanProjectile* TitanProjectile = GetWorld()->SpawnActor<ATitanProjectile>(ProjectileClass, FTransform::Identity))
	// 	{
	// 		// 타이탄 소켓에 부착
	// 		TitanProjectile->AttachToComponent(
	// 			GetTitanBaseFromActorInfo()->GetMesh(),
	// 			FAttachmentTransformRules::SnapToTargetIncludingScale,
	// 			AttatchSocketName
	// 		);
	//
	// 		// 생성된 오브젝트 크기 조정
	// 		TitanProjectile->SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
	//
	// 		// 약간의 랜덤 오프셋 (로컬 기준)
	// 		FVector RandomOffset = FVector(
	// 			FMath::RandRange(-10.0f, 10.0f), // X
	// 			FMath::RandRange(-10.0f, 10.0f), // Y
	// 			FMath::RandRange(-10.0f, 10.0f)  // Z
	// 		);
	//
	// 		// 부착 후 상대 위치 조정
	// 		TitanProjectile->SetActorRelativeLocation(RandomOffset);
	//
	// 		// 필요하다면 회전도 랜덤하게
	// 		FRotator RandomRotation = FRotator(
	// 			FMath::RandRange(-5.0f, 5.0f),
	// 			FMath::RandRange(-5.0f, 5.0f),
	// 			FMath::RandRange(-5.0f, 5.0f)
	// 		);
	// 		TitanProjectile->SetActorRelativeRotation(RandomRotation);
	//
	// 		// 리스트에 추가
	// 		Projectiles.Add(TitanProjectile);
	// 	}
	// }
}
