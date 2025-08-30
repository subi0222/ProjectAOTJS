// Unreal Engine Marketplace Copyright


#include "AOTAnimNotify_HitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAOTAnimNotify_HitCheck::UAOTAnimNotify_HitCheck()
{
}

FString UAOTAnimNotify_HitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAOTAnimNotify_HitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = SkillNum;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
