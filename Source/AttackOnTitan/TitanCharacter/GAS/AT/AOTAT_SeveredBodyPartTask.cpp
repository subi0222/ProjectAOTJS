// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/AT/AOTAT_SeveredBodyPartTask.h"

#include "AbilitySystemComponent.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanBody.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"

UAOTAT_SeveredBodyPartTask* UAOTAT_SeveredBodyPartTask::CreateSeveredBodyPartTask(UGameplayAbility* OwningAbility,
                                                                                  const UAOTTitanSeveredPartComponent* SeveredPartComponent)
{
	UAOTAT_SeveredBodyPartTask* NewSeveredBodyPartTask = NewAbilityTask<UAOTAT_SeveredBodyPartTask>(OwningAbility);
	NewSeveredBodyPartTask->SeveredPartComp = SeveredPartComponent;
	return NewSeveredBodyPartTask;
}

void UAOTAT_SeveredBodyPartTask::Activate()
{
	Super::Activate();
	check(SeveredPartComp);

	//태그 바꿔주고
	if (AbilitySystemComponent.IsValid())
	{
		UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
		if (ASC->HasMatchingGameplayTag(SeveredPartComp->GetRemovedAfterSeveredTitanStateTag()))
			ASC->RemoveLooseGameplayTag(SeveredPartComp->GetRemovedAfterSeveredTitanStateTag());
		
		if (!ASC->HasMatchingGameplayTag(SeveredPartComp->GetStateTagAfterSevered()))
			ASC->AddLooseGameplayTag(SeveredPartComp->GetStateTagAfterSevered());
	}

	//잘라줌
	AAOTTitanBase* ATB = Cast<AAOTTitanBase>(GetAvatarActor());
	if (!ATB)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Titan Base Is Nullptr"));
		return;
	}
	USkeletalMeshComponent* SMC = ATB->GetMesh();
	if (!SMC)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Mesh Is Nullptr"));
		return;
	}
	const USkeletalMeshSocket* Socket = SMC->GetSocketByName(SeveredPartComp->GetAttachSocketName());
	if (!Socket)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Socket Is Nullptr"));
		return;
	}
	FGameplayEffectContextHandle ContextHandle;
	FHitResult HitResult;
	HitResult.Location = SeveredPartComp->GetComponentLocation();
	HitResult.ImpactPoint = SeveredPartComp->GetComponentLocation();
	ContextHandle.AddHitResult(HitResult);
	AbilitySystemComponent->ExecuteGameplayCue(GameplayCue_SeveredBloodSplash, ContextHandle);
	SMC->HideBoneByName(Socket->BoneName, PBO_None);
	FTransform SocketTransform = Socket->GetSocketTransform(SMC);
	AActor* DropActor = GetWorld()->SpawnActor(SeveredPartComp->GetDropActorClass());
	if (DropActor)
	{
		DropActor->SetActorTransform(SocketTransform);
	}
	EndTask();
}

void UAOTAT_SeveredBodyPartTask::StartTask()
{
	ReadyForActivation();
}
