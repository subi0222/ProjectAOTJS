// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotChangedSignature, int32, IndexValue, int32, QuantityValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUsedSignature, int32, IndexValue);
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	/**
	* Broadcasts the initial values to the widget.
	* Derived classes should override this function to implement the specific logic for broadcasting initial values.
	*/
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValue();
	/**
	 * Binds callbacks to the dependencies of the widget controller.
	 * Derived classes should override this function to bind the appropriate callbacks to the game data they are interested in.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSlotChangedSignature OnSlotChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSlotUsedSignature OnSlotUsedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInventoryComponent* InventoryComponent;

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
