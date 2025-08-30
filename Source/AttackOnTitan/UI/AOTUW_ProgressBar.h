// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AOTUW_ProgressBar.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTUW_ProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UAOTUW_ProgressBar(const FObjectInitializer& ObjectInitializer);

	void SetMaxValue(float NewValue);

	void SetValue(float NewValue);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Main;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Delay;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock;
private:
	float MaxValue;
	float CurrentValue;
	float DelayBarPercent = 1.0f;
	float TargetPercent = 1.0f;
	float DelayInterpSpeed = 5.0f;
	bool bShouldTick = false;
};
