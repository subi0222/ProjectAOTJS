// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum.h"
#include "AOTPlayerStat.h"
#include "AOTUW_PlayerStat.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTUW_PlayerStat : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerStat(StatType Type, float NewValue);
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentDamage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackRadius;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackRange;
};
