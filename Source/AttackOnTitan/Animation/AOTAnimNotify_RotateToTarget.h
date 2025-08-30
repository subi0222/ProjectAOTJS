// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AOTAnimNotify_RotateToTarget.generated.h"

/**
 * WarpMotion을 사용하는 몽타주에서 Warp끝나고 그리고 Hit판정 전에 호출하여 몬스터방향으로 캐릭터를 회전시켜 Hit판정이 제대로 이뤄지게 하는 Notify
 * AnimInterface_Player에서 RotateToTarget을 구현하게하여 실행시킴
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_RotateToTarget : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
