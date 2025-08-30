// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AOTAnimNotify_RotateToTarget.generated.h"

/**
 * WarpMotion�� ����ϴ� ��Ÿ�ֿ��� Warp������ �׸��� Hit���� ���� ȣ���Ͽ� ���͹������� ĳ���͸� ȸ������ Hit������ ����� �̷����� �ϴ� Notify
 * AnimInterface_Player���� RotateToTarget�� �����ϰ��Ͽ� �����Ŵ
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_RotateToTarget : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
