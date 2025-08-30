// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInventoryWidget::BroadcastInitValue()
{
	// Cast the generic AttributeSet to the specific USeadenRingAttributeSet
	if (InventoryComponent)
	{
		for (int i = 0; i < InventoryComponent->GetMaxSlotSize(); i++)
		{
			OnSlotChangedDelegate.Broadcast(i, 0);
		}
	}
}

void UInventoryWidget::BindCallbacksToDependencies()
{
	if (InventoryComponent)
	{
		InventoryComponent->SlotChangedDelegate.AddLambda([this](int32 Index, int32 Quantity)
			{
				OnSlotChangedDelegate.Broadcast(Index, Quantity);
			});

		InventoryComponent->SlotUsedDelegate.AddLambda([this](int32 Index)
			{
				OnSlotUsedDelegate.Broadcast(Index);
			});
	}
}

void UInventoryWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}