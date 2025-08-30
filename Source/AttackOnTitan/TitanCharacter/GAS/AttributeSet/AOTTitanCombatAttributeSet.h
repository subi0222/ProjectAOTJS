// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "AOTTitanCombatAttributeSet.generated.h"

class UAOTTitanAbilitySystemComponent;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTTitanCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UAOTTitanAbilitySystemComponent* GetTitanAbilitySystemComponent();
	

public:

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DetectRadius;
	ATTRIBUTE_ACCESSORS(UAOTTitanCombatAttributeSet, DetectRadius)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MeleeAttackDamage;
	ATTRIBUTE_ACCESSORS(UAOTTitanCombatAttributeSet, MeleeAttackDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData SpecialAttackDamage;
	ATTRIBUTE_ACCESSORS(UAOTTitanCombatAttributeSet, SpecialAttackDamage)

	
};
