// Unreal Engine Marketplace Copyright

#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"

UAOTTitanAbilitySystemComponent* UAOTTitanCombatAttributeSet::GetTitanAbilitySystemComponent()
{
	return Cast<UAOTTitanAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
