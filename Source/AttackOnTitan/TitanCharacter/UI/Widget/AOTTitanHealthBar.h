// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "AOTTitanHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTTitanHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetPercent(float Percent);

	virtual void NativeConstruct() override;

public:

	//WBP_Titan�� ���ε�  
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
};



// Unreal Engine Marketplace Copyright

//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/WidgetComponent.h"
//#include "TitanWidgetComponent.generated.h"
//
///**
// *
// */
//UCLASS()
//class ATTACKONTITAN_API UTitanWidgetComponent : public UWidgetComponent
//{
//	GENERATED_BODY()
//
//public:
//
//	void SetupHealthWidget();
//
//	UFUNCTION()
//	void UpdateHealth(FString BoneName, float RemainHpRate);
//
//private:
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
//	TObjectPtr<class UTitanHealthBar> TitanLeftArmHealthBar;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
//	TObjectPtr<class UTitanHealthBar> TitanRightArmHealthBar;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
//	TObjectPtr<class UTitanHealthBar> TitanLeftLegHealthBar;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
//	TObjectPtr<class UTitanHealthBar> TitanRightLegHealthBar;
//
//};
