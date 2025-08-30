// Unreal Engine Marketplace Copyright


#include "Player/AOTPlayerState.h"
#include "GAS/AttributeSet/AOTPlayerAttributeSet.h"
#include "GAS/AOTPlayerAbilitySystemComponent.h"

AAOTPlayerState::AAOTPlayerState()
{
	ASC =CreateDefaultSubobject<UAOTPlayerAbilitySystemComponent>(TEXT("GAS"));
	AttributeSet = CreateDefaultSubobject<UAOTPlayerAttributeSet>(TEXT("AbilitySet"));
}

UAbilitySystemComponent* AAOTPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
