// Unreal Engine Marketplace Copyright


#include "Animation/AOTAnimNotify_SpawnSkillActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Data/AOTSkillData.h"

FString UAOTAnimNotify_SpawnSkillActor::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAOTAnimNotify_SpawnSkillActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = SkillActorNum;
			PayloadData.OptionalObject = Cast<UObject>(SkillData);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
