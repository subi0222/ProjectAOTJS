// Unreal Engine Marketplace Copyright


#include "AOTTitanHealthBar.h"

#include "AttackOnTitan.h"
#include "Components/ProgressBar.h"

void UAOTTitanHealthBar::SetPercent(float Percent)
{
	if (!HealthBar)
	{
		HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	}
	if (!HealthBar)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("There is no ProgressBar Named with HealthBar"))
		return;
	}
	if (Percent >= 0.7f)
	{
		HealthBar->SetFillColorAndOpacity(FLinearColor::Green);
	}
	else if (Percent >= 0.3f)
	{
		HealthBar->SetFillColorAndOpacity(FLinearColor::Yellow);
	}
	else
	{
		HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
	}
	HealthBar->SetPercent(Percent);
}

void UAOTTitanHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	SetPercent(1.0f);
}
