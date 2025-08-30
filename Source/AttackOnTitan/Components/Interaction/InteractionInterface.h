// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class UInteractionComponent;
class UStaticMeshComponent;
/**
 * 
 */
class ATTACKONTITAN_API IInteractionInterface
{
	GENERATED_BODY()

	//////////////////// Please inherit this interface class for classes that need to interact
public:
	// ������ �̸��� ��ȯ�ϴ� �޼���
	virtual FString GetInteractionName() const = 0;

	// run this function when the interaction starts 
	virtual void BeginInteract(UInteractionComponent* InteractionComponent) = 0;

	// run this function when the interaction ends
	virtual void EndInteract(UInteractionComponent* InteractionComponent) = 0;

	// run this function when the player presses the interaction input
	virtual void Interact(UInteractionComponent* InteractionComponent) = 0;

	// ��ȣ�ۿ� ���¸� �����ϴ� ��ƿ��Ƽ �Լ� (���� ���ü�, �ؽ�Ʈ, ���ٽ� �� ����)
	virtual void SetInteractionState(UInteractionComponent* InteractionComponent, UStaticMeshComponent* MeshComponent, ESlateVisibility Visibility, const FText& WidgetText, bool bEnableStencil, int32 StencilValue);
};
