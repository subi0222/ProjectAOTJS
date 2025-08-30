// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AOTAnimNotify_SpawnSkillActor.generated.h"

class UAOTSkillData;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_SpawnSkillActor : public UAnimNotify
{
	GENERATED_BODY()
	protected:
    	virtual FString GetNotifyName_Implementation() const override;
    	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
    
    protected:
    	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
    	FGameplayTag TriggerGameplayTag;
    	UPROPERTY(EditAnywhere)
    	TObjectPtr<UAOTSkillData> SkillData;
    	UPROPERTY(EditAnywhere)
    	int32 SkillActorNum = 0;
};
