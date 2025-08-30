// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseInteractionItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Interaction/InteractionComponent.h"
#include "Character/BaseCharacter.h"
#include "AttackOnTitan.h"
#include "Components/Widget.h"

ABaseInteractionItem::ABaseInteractionItem()
{
    PrimaryActorTick.bCanEverTick = false;

    //bReplicates = true;
    //NetUpdateFrequency = 20.0f;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    StaticMesh->SetupAttachment(GetRootComponent());
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABaseInteractionItem::BeginPlay()
{
    Super::BeginPlay();

    InitializeItem(1);
}

void ABaseInteractionItem::InitializeItem(int32 InQuantity)
{
    if (ItemDataTable && !ItemID.IsNone())
    {
        static const FString ContextString(TEXT("Item Context"));
        PickupItemData = ItemDataTable->FindRow<FPickupItemData>(ItemID, ContextString, true);
        Quantity = InQuantity;
    }
}

void ABaseInteractionItem::BeginInteract(UInteractionComponent* InteractionComponent)
{
    if (InteractionComponent)
    {
        // Default interaction start logic
        SetInteractionState(InteractionComponent, StaticMesh, ESlateVisibility::Visible, FText::FromName(ItemID), false, 0);
    }
}

void ABaseInteractionItem::EndInteract(UInteractionComponent* InteractionComponent)
{
    if (InteractionComponent)
    {
        // Default interaction end logic
        SetInteractionState(InteractionComponent, StaticMesh, ESlateVisibility::Hidden, FText(), false, 0);
    }
}

void ABaseInteractionItem::Interact(UInteractionComponent* InteractionComponent)
{
    if (InteractionComponent)
    {
        // Default interaction logic, can be overridden by subclasses if needed
        SetInteractionState(InteractionComponent, StaticMesh, ESlateVisibility::Collapsed, FText(), false, 0);
        // Optional: Override base class interaction logic
        ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(InteractionComponent->GetOwner());
        if (BaseCharacter)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Interact"));
            BaseCharacter->Pickup(this);
        }
    }
}

FString ABaseInteractionItem::GetInteractionName() const
{
    // Return a default interaction name
    return TEXT("Base Interaction Item");
}

void ABaseInteractionItem::SetInteractionState(UInteractionComponent* InteractionComponent, UStaticMeshComponent* MeshComponent, ESlateVisibility Visibility, const FText& WidgetText, bool bEnableStencil, int32 StencilValue)
{
    if (InteractionComponent)
    {
        // Set widget visibility and text
        InteractionComponent->SetWidgetVisibiity(Visibility);
        InteractionComponent->SetWidgetText(WidgetText);

        // Set stencil for highlighting or other visual feedback
        APawn* InteractingPawn = Cast<APawn>(InteractionComponent->GetOwner());
        if (InteractingPawn && InteractingPawn->IsLocallyControlled() && MeshComponent)
        {
            
            FString mName = MeshComponent->GetName();
            FString oName = MeshComponent->GetOwner()->GetName();
            MeshComponent->SetRenderCustomDepth(bEnableStencil);
            MeshComponent->SetCustomDepthStencilValue(StencilValue);
        }     
    }
}

