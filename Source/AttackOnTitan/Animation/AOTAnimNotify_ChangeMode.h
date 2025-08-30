// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AOTAnimNotify_ChangeMode.generated.h"

/**
 * ModeChange 몽타주에 정말로 무기를 바꿔껴야하는 위치에 Notify로 설정하는 클래스
 */
UCLASS()
class ATTACKONTITAN_API UAOTAnimNotify_ChangeMode : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
