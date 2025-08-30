// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AOTAnimNotify_ChangeMode.generated.h"

/**
 * ModeChange ��Ÿ�ֿ� ������ ���⸦ �ٲ㲸���ϴ� ��ġ�� Notify�� �����ϴ� Ŭ����
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_ChangeMode : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
