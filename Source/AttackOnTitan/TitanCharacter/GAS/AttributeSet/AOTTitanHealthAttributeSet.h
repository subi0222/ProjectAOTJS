// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "AOTTitanHealthAttributeSet.generated.h"

class UAOTTitanAbilitySystemComponent;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfHealthDelegate);
UCLASS()
class ATTACKONTITAN_API UAOTTitanHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
private:
	void ApplyDamage(float DamageAmount);

	void ApplyStaminaCost(float StaminaCostAmount);

	void ApplyStamina(float StaminaAmount);

public:

	UAOTTitanAbilitySystemComponent* GetTitanAbilitySystemComponent();

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;



public:

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UAOTTitanHealthAttributeSet, HP)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAOTTitanHealthAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAOTTitanHealthAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData StaminaCost;
	ATTRIBUTE_ACCESSORS(UAOTTitanHealthAttributeSet, StaminaCost)

};
