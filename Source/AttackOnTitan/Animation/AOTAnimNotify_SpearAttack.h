// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AOTAnimNotify_SpearAttack.generated.h"

/**
 * SpearAttackBase ��Ÿ�ֿ� Spear�� �����Ǿ�� �� ������ ���� NotifyŬ���� Event.Character.SpawnSpear�� Trigger�ؼ� GameAbility����
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_SpearAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;
};
