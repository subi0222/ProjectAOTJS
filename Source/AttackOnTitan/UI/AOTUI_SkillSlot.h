// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AOTUI_SkillSlot.generated.h"

class UMaterialInterface;
class UImage;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTUI_SkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void StartCooldown(float Duration);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TObjectPtr<UMaterialInterface> BaseMaterial;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SkillImage;

private:
    TObjectPtr<UMaterialInstanceDynamic> CooldownMaterialInstance;
    float CooldownDuration = 0.f;
    float ElapsedTime = 0.f;
    FTimerHandle CooldownTimerHandle;
    bool bStartTick = false;
};
