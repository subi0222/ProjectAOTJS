// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AOTTitanBodyTargetWidgetComponent.generated.h"

class UAOTTitanSeveredPartComponent;
class AAOTTitanBase;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTTitanBodyTargetWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UAOTTitanBodyTargetWidgetComponent();

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ChangeTargetWidgetTo(UAOTTitanSeveredPartComponent* SeveredPartComponent);

	UFUNCTION()
	void TurnOffTargetWidget(UAOTTitanSeveredPartComponent* SeveredPartComponent);

	UFUNCTION()
	void HandleTitanOnDie();

protected:

	FLinearColor GetLinearColor(UAOTTitanSeveredPartComponent* SeveredPartComp) const;

protected:

	UPROPERTY(EditAnywhere, Category = Color)
	FLinearColor HPSafeColor;

	UPROPERTY(EditAnywhere, Category = Color)
	FLinearColor HPWarningColor;

	UPROPERTY(EditAnywhere, Category = Color)
	FLinearColor HPCriticalColor;

	UPROPERTY(EditAnywhere, Category = Threshold)
	float HPSafeThreshold;

	UPROPERTY(EditAnywhere, Category = Threshold)
	float HPWarningThreshold;

	UPROPERTY(EditAnywhere, Category = Threshold)
	float HPCriticalThreshold;

	UPROPERTY(EditAnywhere, Category = Size);
	float MaxSize;

	UPROPERTY(EditAnywhere, Category = Size);
	float MinSize;

	UPROPERTY(EditAnywhere, Category = UI);
	float VisibleLength;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;	
	
};