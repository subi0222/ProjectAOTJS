// Unreal Engine Marketplace Copyright


#include "Components/Inventory/InventoryComponent.h"
#include "Item/BaseInteractionItem.h"
#include "Character/BaseCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->HasAuthority())
	{
		for (int i = 0; i < MaxSlotSize; i++)
		{
			FItemSlot NewSlot = FItemSlot();
			NewSlot.ItemType = EItemType::EIT_None;
			ItemSlots.Add(NewSlot);
		}
	}
}


bool UInventoryComponent::AddItemSlot(FPickupItemData ItemData, int32 Quantity)
{
	bAdded = false;
	FoundIndex = FindItemSlot(ItemData, Quantity);
	if (FoundIndex == -1) return bAdded;

	if (ItemSlots[FoundIndex].ItemType == EItemType::EIT_None)
	{
		ItemSlots[FoundIndex].ItemName = ItemData.ItemName;
		ItemSlots[FoundIndex].ItemType = ItemData.ItemType;
		ItemSlots[FoundIndex].Thumbnail = ItemData.Thumbnail;
		ItemSlots[FoundIndex].StackSize = ItemData.StackSize;
		ItemSlots[FoundIndex].Quantity += Quantity;
		//ItemSlots[FoundIndex].ItemEffect = ItemData.Effect;

		SlotChangedDelegate.Broadcast(FoundIndex, ItemSlots[FoundIndex].Quantity);
		bAdded = true;
	}
	else
	{
		// 찾을경우
		ItemSlots[FoundIndex].Quantity += Quantity;

		SlotChangedDelegate.Broadcast(FoundIndex, ItemSlots[FoundIndex].Quantity);
		bAdded = true;
	}
	return bAdded;
}

void UInventoryComponent::UseItemSlot(int32 Index)
{
	if (ItemSlots[Index].ItemType == EItemType::EIT_None || ItemSlots[Index].Quantity <= 0)
	{
		return;
	}
	else
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Client"));
		if (Player)
		{
			if (ItemSlots[Index].ItemType == EItemType::EIT_Potion)
			{
				//if (Player->ActionState == EActionState::EAS_Unoccupied)
				//{
				//	Player->ApplyEffectToSelf(ItemSlots[Index].ItemEffect, Player->GetPlayerLevel());
				ItemSlots[Index].Quantity -= 1;
				if (ItemSlots[Index].Quantity <= 0)
				{
					DeleteItemSlot(Index);
				}
				FoundIndex = Index;
				SlotChangedDelegate.Broadcast(Index, ItemSlots[Index].Quantity);
				SlotUsedDelegate.Broadcast(Index);
			}
		}
	}
}

void UInventoryComponent::DeleteItemSlot(int32 Index)
{
	FItemSlot NewSlot = FItemSlot();
	ItemSlots[Index] = NewSlot;
	FoundIndex = Index;
	SlotChangedDelegate.Broadcast(Index, ItemSlots[Index].Quantity);
}

int32 UInventoryComponent::FindItemSlot(const FPickupItemData ItemData, int32 Quantity, bool bLogNotFound) const
{
	int index = 0;
	for (const FItemSlot& Struct : ItemSlots)
	{
		// TODO : BUG
		if (Struct.ItemName == ItemData.ItemName)
		{
			if (Struct.Quantity + Quantity <= Struct.StackSize)
			{
				return index;
			}
		}
		index++;
	}

	index = 0;
	for (const FItemSlot& Struct : ItemSlots)
	{
		// TODO : BUG
		if (Struct.ItemType == EItemType::EIT_None)
		{
			return index;
		}
		index++;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Info for ItemID [%s] on AttributeInfo[%s]"), *ItemData.ItemName.ToString(), *GetNameSafe(this));
	}
	return -1;
}
