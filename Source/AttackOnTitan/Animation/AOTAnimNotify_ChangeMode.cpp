// Unreal Engine Marketplace Copyright


#include "Animation/AOTAnimNotify_ChangeMode.h"
#include "AttackOnTitan/Interface/AOTAnimInterface_Player.h"

void UAOTAnimNotify_ChangeMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAOTAnimInterface_Player* Interface = Cast<IAOTAnimInterface_Player>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->ChangeMode();
	}
}
