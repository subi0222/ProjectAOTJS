// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum.h"
#include "AOTPlayerStat.h"
#include "AOTUW_PlayerBar.generated.h"

class UVerticalBox;
class UAOTUW_ProgressBar;
class UAOTUW_PlayerStat;
class UAOTUI_SkillSlot;

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTUW_PlayerBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; // ���� �ʱ�ȭ �� ����
	
	UFUNCTION()
	void SetHealthBar(float Current, float Max);
	UFUNCTION()
	void SetGasBar(float Current, float Max);
	UFUNCTION()
	void SetSkillCoolTime(int SkillNum,float CoolTime);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUW_ProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUW_ProgressBar> GasBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUW_PlayerStat> PlayerStat;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUI_SkillSlot> Skill01;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUI_SkillSlot> Skill02;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUI_SkillSlot> Skill03;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAOTUI_SkillSlot> Skill04;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;
};
