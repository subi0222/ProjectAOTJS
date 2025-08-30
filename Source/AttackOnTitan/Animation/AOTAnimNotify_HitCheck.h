// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AOTAnimNotify_HitCheck.generated.h"

/**
 * �⺻���ݿ��� Hit������ �ִ� Notify Ŭ����. TriggerTag�� Event.Character.HitCheck �ߵ���Ų��.
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_HitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAOTAnimNotify_HitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag TriggerGameplayTag;
	UPROPERTY(EditAnywhere)
	int32 SkillNum = 0;
};
