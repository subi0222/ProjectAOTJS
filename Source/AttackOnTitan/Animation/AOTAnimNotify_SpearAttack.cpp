// Unreal Engine Marketplace Copyright


#include "Animation/AOTAnimNotify_SpearAttack.h"
#include "AbilitySystemBlueprintLibrary.h"


FString UAOTAnimNotify_SpearAttack::GetNotifyName_Implementation() const
{
	return TEXT("SpearAttack");
}

void
UAOTAnimNotify_SpearAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
