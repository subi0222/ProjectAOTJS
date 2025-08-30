// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanAttackPartComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TitanCharacter/AOTTitanBase.h"

UAOTTitanAttackPartComponent::UAOTTitanAttackPartComponent()
{
	bWantsInitializeComponent = true;
}

void UAOTTitanAttackPartComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AAOTTitanBase* TB = Cast<AAOTTitanBase>(GetOwner());
	if (!TB)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Titan Base Is Null"))
		return;
	}
	TitanBase = TB;
}

void UAOTTitanAttackPartComponent::MakeAttackHitCheckEvent()
{
	FGameplayEventData EventData;
	
	EventData.EventTag = Titan_Event_AttackHitCheck;
	EventData.Target = TitanBase;
	EventData.OptionalObject = this;

	ensure(TitanBase != nullptr);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		TitanBase, 
		Titan_Event_AttackHitCheck, 
		EventData);
}

void UAOTTitanAttackPartComponent::RegisterHitCharacter(ABaseCharacter* HitCharacter)
{
	if (HitCharacters.Contains(HitCharacter))
		return;
	HitCharacters.Add(HitCharacter);
}
