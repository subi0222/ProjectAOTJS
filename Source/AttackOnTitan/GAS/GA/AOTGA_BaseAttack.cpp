// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_BaseAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/BaseCharacter.h"
#include "Data/AOTBaseComboAttackData.h"
#include "GAS/AOTPlayerAbilitySystemComponent.h"

UAOTGA_BaseAttack::UAOTGA_BaseAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_BaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = CastChecked<ABaseCharacter>(ActorInfo->AvatarActor);
	CurrentComboData = Character->GetBaseComboAttackData();
	OwnerController = Character->GetController();
	if (OwnerController)
	{
		FRotator CameraRotation = OwnerController->GetControlRotation();
		FRotator NewRotation(0.0f, CameraRotation.Yaw, 0.0f);
		Character->SetActorRotation(NewRotation);
	}

	CurrentCombo = 1;
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("BaseAttack"), Character->GetBaseAttackMontage());
	Task->OnCompleted.AddDynamic(this, &UAOTGA_BaseAttack::OnCompleteCallback);
	Task->ReadyForActivation();

	float CanInputTime = CurrentComboData->ComboInfo[CurrentCombo - 1].CanInputTime;
	float FailInputTime = CurrentComboData->ComboInfo[CurrentCombo - 1].FailInputTime;
	GetWorld()->GetTimerManager().SetTimer(CanInputTimerHandle, this, &UAOTGA_BaseAttack::CanInputTimeCallback, CanInputTime,false);
	GetWorld()->GetTimerManager().SetTimer(FailInputTimerHandle,this, &UAOTGA_BaseAttack::FailInputTimeCallback, FailInputTime,false);
}

void UAOTGA_BaseAttack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	if (CanNextInput == true)
	{
		NextCombo();
	}
}

void UAOTGA_BaseAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAOTGA_BaseAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	CurrentComboData = nullptr;
	CanNextInput = false;
	CanInputTimerHandle.Invalidate();
	FailInputTimerHandle.Invalidate();
	CurrentCombo = -1;
}

void UAOTGA_BaseAttack::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAOTGA_BaseAttack::NextCombo()
{
	if (CurrentCombo != CurrentComboData->ComboCount)
	{
		CurrentCombo++;
		float CanInputTime = CurrentComboData->ComboInfo[CurrentCombo - 1].CanInputTime;
		float FailInputTime = CurrentComboData->ComboInfo[CurrentCombo - 1].FailInputTime;
		FName NextSectorName = GetNextSector();
		APawn* Avatar = Cast<APawn>(GetAvatarActorFromActorInfo());
		if (Avatar)
		{
			AController* Controller = Avatar->GetController();
			FRotator CameraRotation = Controller->GetControlRotation();
			FRotator NewRotation(0.0f, CameraRotation.Yaw, 0.0f);
			Avatar->SetActorRotation(NewRotation);
		}
	
		MontageJumpToSection(NextSectorName);
		CanNextInput = false;
		GetWorld()->GetTimerManager().SetTimer(CanInputTimerHandle,this, &UAOTGA_BaseAttack::CanInputTimeCallback, CanInputTime,false);
		GetWorld()->GetTimerManager().SetTimer(FailInputTimerHandle, this, &UAOTGA_BaseAttack::FailInputTimeCallback, FailInputTime,false);
	}
}

void UAOTGA_BaseAttack::CanInputTimeCallback()
{
	CanNextInput = true;
}

void UAOTGA_BaseAttack::FailInputTimeCallback()
{
	CanNextInput = false;
}

FName UAOTGA_BaseAttack::GetNextSector()
{
	FName SectorName = *FString::Printf(TEXT("%s%d"), *CurrentComboData->Prefix, CurrentCombo);
	return SectorName;
}
