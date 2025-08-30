// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameplayEffectTypes.h"
#include "AOTTitanHPWidgetComponent.generated.h"

/**
 * 
 */

class AAOTTitanBase;
class UAOTTitanHealthBar;

UCLASS()
class ATTACKONTITAN_API UAOTTitanHPWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UAOTTitanHPWidgetComponent();

	void Update(const FOnAttributeChangeData& AttributeChangeData);

	virtual void InitWidget() override;

	virtual void InitializeComponent() override;

	UFUNCTION()
	void Update(float Percent);

private:

	FGameplayAttribute GetHPAttribute() const;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UAOTTitanHealthBar> HealthBar;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;
	
};
