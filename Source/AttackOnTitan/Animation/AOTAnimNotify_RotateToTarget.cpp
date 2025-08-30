// Unreal Engine Marketplace Copyright


#include "Animation/AOTAnimNotify_RotateToTarget.h"
#include "Interface/AOTAnimInterface_Player.h"

void UAOTAnimNotify_RotateToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IAOTAnimInterface_Player* Interface = Cast<IAOTAnimInterface_Player>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->RotateToTarget();
	}
}
