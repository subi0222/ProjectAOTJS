// Unreal Engine Marketplace Copyright


#include "GAS/AttributeSet/AOTPlayerAttributeSet.h"
#include "GameplayEffectExtension.h"

UAOTPlayerAttributeSet::UAOTPlayerAttributeSet()
	: AttackPower(10.f)
	, MaxAttackPower(10.f)
	, Health(100.f)
	, MaxHealth(100.f)
	, Gas(100.f)
	, MaxGas(100.f)
	, Damage(0.f)
	, MaxDamage(0.f)
	, AttackRange(50.f)
	, MaxAttackRange(50.f)
	, AttackRadius(50.f)
	, MaxAttackRadius(50.f)
{
}

void UAOTPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
		NewValue = NewValue < 0.f ? 0.f : NewValue;
}

bool UAOTPlayerAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UAOTPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float PrevHealth = GetHealth();
		float CurrentHealth = FMath::Clamp(GetHealth() - GetDamage(), 0.f, GetMaxHealth());
		SetHealth(CurrentHealth);
		SetDamage(0.f);
		OnChangeHealth.Broadcast(GetHealth(), GetMaxHealth());

		UE_LOG(LogTemp, Error, TEXT("%f -> %f"), PrevHealth, CurrentHealth);
	}
	else if (Data.EvaluatedData.Attribute == GetGasAttribute())
	{
		OnChangeGas.Broadcast(GetGas(), GetMaxGas());
	}
	else if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
		OnChangeAttackPower.Broadcast(GetAttackPower());
	else if (Data.EvaluatedData.Attribute == GetAttackRadiusAttribute())
		OnChangeAttackRadius.Broadcast(GetAttackRadius());
	else if (Data.EvaluatedData.Attribute == GetAttackRangeAttribute())
		OnChangeAttackRange.Broadcast(GetAttackRange());
}
