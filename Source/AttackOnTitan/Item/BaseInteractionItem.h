// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Interaction/InteractionInterface.h"
#include "Enum.h"
#include "EffectActor.h"
#include "BaseInteractionItem.generated.h"

class UInteractionComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FPickupItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    // Thumbnail image for the weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Thumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    // Stack size (number of items that can be stacked together)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 StackSize;
};



UCLASS()
class ATTACKONTITAN_API ABaseInteractionItem : public AEffectActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseInteractionItem();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // ID of the item in the DataTable
    UPROPERTY(EditAnywhere, Category = "Item")
    FName ItemID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Quantity = 1;

    // DataTable that holds the weapon's item data
    UPROPERTY(EditAnywhere, Category = "Item")
    UDataTable* ItemDataTable;

    FPickupItemData* PickupItemData;
public:
    // Initializes the weapon item based on the DataTable information
    UFUNCTION(BlueprintCallable)
    void InitializeItem(int32 InQuantity);

    /** Implements the interaction interface */
    virtual void BeginInteract(UInteractionComponent* InteractionComponent) override;
    virtual void EndInteract(UInteractionComponent* InteractionComponent) override;
    virtual void Interact(UInteractionComponent* InteractionComponent) override;

    // Provides a default implementation for interaction name retrieval
    virtual FString GetInteractionName() const override;

    // Provides a default implementation for setting interaction state
    virtual void SetInteractionState(UInteractionComponent* InteractionComponent, UStaticMeshComponent* MeshComponent, ESlateVisibility Visibility, const FText& WidgetText, bool bEnableStencil, int32 StencilValue) override;


    FORCEINLINE  FPickupItemData* GetItemData() { return PickupItemData;  };
    FORCEINLINE  int32 GetItemQuantity() { return Quantity; };

protected:

    // Mesh component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* StaticMesh;
};
