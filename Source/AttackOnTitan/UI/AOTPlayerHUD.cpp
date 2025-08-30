// Unreal Engine Marketplace Copyright


#include "UI/AOTPlayerHUD.h"
#include "Character/BaseCharacter.h"
#include "UI/AOTUW_PlayerBar.h"
#include "Components/Interaction/InteractionWidget.h"
#include "Enum.h"
#include "Kismet/GameplayStatics.h"

AAOTPlayerHUD::AAOTPlayerHUD()
{
}

void AAOTPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{

		PlayerBar = CreateWidget<UAOTUW_PlayerBar>(GetWorld(), PlayerBarClass);
		if (PlayerBar)
		{
			PlayerBar->Owner = Player;
			PlayerBar->AddToViewport();
		}
	}
}

UInteractionWidget* AAOTPlayerHUD::GetInteractionWidget()
{
	if (InteractionWidget == nullptr)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetOwningPlayerController(), InteractionWidgetClass);
		InteractionWidget->AddToViewport();
	}
	return InteractionWidget;
}

