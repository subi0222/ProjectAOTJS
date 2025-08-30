// Unreal Engine Marketplace Copyright


#include "TitanCharacter/UI/AOTTitanHPWidgetComponent.h"

#include "AttackOnTitan.h"
#include "TitanCharacter/UI/Widget/AOTTitanHealthBar.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanHealthAttributeSet.h"


UAOTTitanHPWidgetComponent::UAOTTitanHPWidgetComponent()
{
	bWantsInitializeComponent = true;
}

void UAOTTitanHPWidgetComponent::Update(const FOnAttributeChangeData& AttributeChangeData)
{
	if (!HealthBar)
	{
		HealthBar = Cast<UAOTTitanHealthBar>(GetUserWidgetObject());
	}
	float Percent = AttributeChangeData.NewValue / AttributeChangeData.OldValue;
	HealthBar->SetPercent(Percent);
}

void UAOTTitanHPWidgetComponent::InitWidget()
{
	Super::InitWidget();
	// if (!HealthBar)
	// {
	// 	HealthBar = Cast<UTitanHealthBar>(GetUserWidgetObject());
	// }
	// if (HealthBar)
	// {
	// 	AOT_LOG(LogAOT, Log, TEXT("Titan Health Bar : %s"), *HealthBar->GetName());
	// 	HealthBar->SetPercent(1.0f);
	// }
}

void UAOTTitanHPWidgetComponent::Update(float Percent)
{
	if (!HealthBar)
	{
		HealthBar = Cast<UAOTTitanHealthBar>(GetUserWidgetObject());
	}
	HealthBar->SetPercent(Percent);
}

void UAOTTitanHPWidgetComponent::InitializeComponent()
{
	Super::InitializeComponent();
	TitanBase = Cast<AAOTTitanBase>(GetOwner());
	if (WidgetClass)
	{
		FString WidgetClassName = WidgetClass->GetName();
		AOT_LOG(LogAOT, Log, TEXT("Widget Name %s"), *WidgetClassName)
	}
}

FGameplayAttribute UAOTTitanHPWidgetComponent::GetHPAttribute() const
{
	return TitanBase->GetAbilitySystemComponent()->GetSet<UAOTTitanHealthAttributeSet>()->GetHPAttribute();
}
