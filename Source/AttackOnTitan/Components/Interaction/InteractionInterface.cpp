// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionComponent.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.
void IInteractionInterface::SetInteractionState(UInteractionComponent* InteractionComponent, UStaticMeshComponent* MeshComponent, ESlateVisibility Visibility, const FText& WidgetText, bool bEnableStencil, int32 StencilValue)
{
    if (InteractionComponent)
    {
        // ���� ���ü� ����
        InteractionComponent->SetWidgetVisibiity(Visibility);

        // ���� �ؽ�Ʈ ����
        InteractionComponent->SetWidgetText(WidgetText);

        // ���ٽ� ����
        APawn* InteractingPawn = Cast<APawn>(InteractionComponent->GetOwner());
        if (InteractingPawn && InteractingPawn->IsLocallyControlled())
        {
            if (MeshComponent)
            {
                MeshComponent->SetRenderCustomDepth(bEnableStencil);
                MeshComponent->SetCustomDepthStencilValue(StencilValue);
            }
        }
    }
}