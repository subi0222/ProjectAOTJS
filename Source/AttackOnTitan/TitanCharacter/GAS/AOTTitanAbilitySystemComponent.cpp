#include "AOTTitanAbilitySystemComponent.h"

#include "AOTGameplayTag.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/GAS/GA/AOTGA_TitanRangedAttack.h"



void UAOTTitanAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	TitanOwner = Cast<AAOTTitanBase>(InAvatarActor);
	if (!HasMatchingGameplayTag(Titan_State_Neck_Alive))
		AddLooseGameplayTag(Titan_State_Neck_Alive, 1);

	if (!HasMatchingGameplayTag(Titan_State_Arm_TwoArm))
		AddLooseGameplayTag(Titan_State_Arm_TwoArm, 1);

	if (!HasMatchingGameplayTag(Titan_State_Leg_TwoLeg))
		AddLooseGameplayTag(Titan_State_Leg_TwoLeg, 1);
}

