// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AOTPlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeHealth, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeGas, float, CurrentGas, float, MaxGas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeAttackPower, float, CurrentAttackPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeAttackRange, float, CurrentAttackRanage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeAttackRadius, float, CurrentAttackRadius);



/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
		UAOTPlayerAttributeSet();
    
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, AttackRange);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxAttackRange);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, AttackRadius);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxAttackRadius);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, Health);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxHealth);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, Gas);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxGas);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, Damage);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxDamage);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, AttackPower);
    	ATTRIBUTE_ACCESSORS(UAOTPlayerAttributeSet, MaxAttackPower);
    
    	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
    	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
		virtual	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    
    protected:
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData AttackRange;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxAttackRange;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData AttackRadius;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxAttackRadius;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData Health;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxHealth;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData Gas;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxGas;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData Damage;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxDamage;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData AttackPower;
    	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
    	FGameplayAttributeData MaxAttackPower;	
    
    public:
    	FOnChangeHealth OnChangeHealth;
    	FOnChangeGas OnChangeGas;
		FOnChangeAttackPower OnChangeAttackPower;
		FOnChangeAttackRange OnChangeAttackRange;
		FOnChangeAttackRadius OnChangeAttackRadius;
};
