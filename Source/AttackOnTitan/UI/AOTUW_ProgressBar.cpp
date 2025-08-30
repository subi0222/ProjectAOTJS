// Unreal Engine Marketplace Copyright


#include "UI/AOTUW_ProgressBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UAOTUW_ProgressBar::UAOTUW_ProgressBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	
{

}

void UAOTUW_ProgressBar::SetMaxValue(float NewValue)
{
	if (NewValue <= 0.f)
		return;
	MaxValue = NewValue;

	if (CurrentValue > MaxValue)
		CurrentValue = MaxValue;

	ProgressBar_Main->SetPercent(CurrentValue / MaxValue);
	ProgressBar_Delay->SetPercent(CurrentValue / MaxValue);
	DelayBarPercent = CurrentValue / MaxValue;
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentValue, MaxValue)));
}

void UAOTUW_ProgressBar::SetValue(float NewValue)
{
	ensure(MaxValue > 0.f);
	CurrentValue = FMath::Clamp(NewValue, 0.f, MaxValue);
	ProgressBar_Main->SetPercent(CurrentValue / MaxValue);
	TargetPercent = CurrentValue / MaxValue;
	if (DelayBarPercent < TargetPercent)
	{
		DelayBarPercent = TargetPercent;
		ProgressBar_Delay->SetPercent(DelayBarPercent);
	}
	else
		bShouldTick = true;
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentValue, MaxValue)));
}

void UAOTUW_ProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	MaxValue = 100.f;
	CurrentValue = MaxValue;
	SetValue(CurrentValue);
}

void UAOTUW_ProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bShouldTick)
	{
		if (DelayBarPercent > TargetPercent && ProgressBar_Delay)
		{
			DelayBarPercent = FMath::FInterpTo(DelayBarPercent, TargetPercent, InDeltaTime, DelayInterpSpeed);
			ProgressBar_Delay->SetPercent(DelayBarPercent);
		}
		else
			bShouldTick = false;
	}
}




