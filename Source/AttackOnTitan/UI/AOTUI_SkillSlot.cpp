// Unreal Engine Marketplace Copyright


#include "UI/AOTUI_SkillSlot.h"
#include "Components/Image.h"

void UAOTUI_SkillSlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (SkillImage && BaseMaterial)
    {      
        CooldownMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        SkillImage->SetBrushFromMaterial(CooldownMaterialInstance);
    }
}

void UAOTUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (bStartTick)
    {
        ElapsedTime += InDeltaTime;
        float Progress = FMath::Clamp(ElapsedTime / CooldownDuration, 0.f, 1.0f);
        CooldownMaterialInstance->SetScalarParameterValue("percent", Progress);
        if (ElapsedTime >= CooldownDuration)
            bStartTick = false;
    }
}


void UAOTUI_SkillSlot::StartCooldown(float Duration)
{
    CooldownDuration = Duration;
    ElapsedTime = 0.0f;
    bStartTick = true;
}
