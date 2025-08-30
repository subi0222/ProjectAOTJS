// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AOTPlayerHUD.generated.h"


class UAOTUW_PlayerBar;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTPlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AAOTPlayerHUD();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	class UInteractionWidget* GetInteractionWidget();

public:
	UPROPERTY()
	TObjectPtr<UAOTUW_PlayerBar> PlayerBar;

private:
	UPROPERTY(EditAnywhere, Category = PlayerBar)
	TSubclassOf<UUserWidget> PlayerBarClass;	

	UPROPERTY(EditAnywhere, Category = PlayerStat)
	TSubclassOf<UUserWidget> PlayerStatClass;

	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> InteractionWidgetClass;

	class UInteractionWidget* InteractionWidget;
};
