// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotChanged, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlotUsed, int32);

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StackSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Thumbnail;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<class UGameplayEffect> ItemEffect;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemSlot> ItemSlots;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxSlotSize = 8;

	UPROPERTY()
	int32 FoundIndex = 0;
public:
	UPROPERTY()
	bool bAdded = false;

	UFUNCTION(BlueprintCallable)
	bool AddItemSlot(struct FPickupItemData ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void UseItemSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DeleteItemSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	int32 FindItemSlot(const struct FPickupItemData ItemData, int32 Quantity, bool bLogNotFound = false) const;

	FOnSlotChanged SlotChangedDelegate;

	FOnSlotUsed SlotUsedDelegate;

	FORCEINLINE int32 GetMaxSlotSize() { return MaxSlotSize; }

};
