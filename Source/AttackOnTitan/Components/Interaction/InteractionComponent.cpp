// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BaseCharacter.h"
#include "InteractionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AttackOnTitan.h"
#include "InteractionInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceInteraction();
}

/////// Interaction

// Performs a trace to detect interactable objects
void UInteractionComponent::TraceInteraction()
{
	/*
	* Set the start and end locations for the trace
	*/
	FVector StartLocation = GetOwner()->GetActorLocation() - StartOffset;
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * TraceDistance);

	// Configure the trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("ItemTrace")), true, GetOwner());
	MakeTraceParams(TraceParams);

	FHitResult HitResult;

	// Perform a sphere trace to detect objects
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		Interactable,
		FCollisionShape::MakeSphere(TraceRadius),
		TraceParams
	);

	// ����� ���Ǿ� �׸���
	/*
	* 	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 12, FColor::Green, false, 1.f);
	*/

	if (bHit)
	{
		if (HitResult.GetActor())
		{
			DetectedActor = HitResult.GetActor();
			RunInteraction(bHit);
		}
	}
	else
	{
		RunInteraction(bHit);
		DetectedActor = nullptr;
	}
}

// Executes the interaction logic with the detected object
void UInteractionComponent::RunInteraction(bool bHit)
{
	if (bHit)
	{
		if (DetectedActor)
		{
			// Attempt to cast the detected actor to the interaction interface
			IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(DetectedActor);
			if (InteractionInterface)
			{
				InteractionInterface->BeginInteract(this);
			}
		}
	}
	else
	{
		// If no hit, end the interaction with the previously detected actor
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(DetectedActor);
		if (InteractionInterface)
		{
			InteractionInterface->EndInteract(this);
		}
	}
}

// Configures the trace parameters
void UInteractionComponent::MakeTraceParams(FCollisionQueryParams& OutTraceParams)
{
	// Set trace parameters for sphere trace
	OutTraceParams.bTraceComplex = true;
	OutTraceParams.bReturnPhysicalMaterial = false;

	/*
	* Add actors to ignore during the trace
	*/
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	AddActorsToIgnore(ActorsToIgnore);

	OutTraceParams.AddIgnoredActors(ActorsToIgnore);
}

void UInteractionComponent::AddActorsToIgnore(TArray<AActor*>& ActorsToIgnore)
{
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
}

// Adds actors to the ignore list for the trace
void UInteractionComponent::AddIgnoreActor(AActor* newActor)
{
	IgnoreActors.Add(newActor);
}

// Removes a single actor from the ignore list
void UInteractionComponent::RemoveIgnoreActor(AActor* newActor)
{
	IgnoreActors.Remove(newActor);
}

void UInteractionComponent::SetInteractWidget(UInteractionWidget* InWidget)
{
	InteractWidget = InWidget;
}

void UInteractionComponent::SetWidgetVisibiity(ESlateVisibility Visibity)
{
	if (InteractWidget)
	{
		InteractWidget->SetVisibility(Visibity);
	}
}

void UInteractionComponent::SetWidgetText(FText Text)
{
	if (InteractWidget)
	{
		InteractWidget->ActorNameText->SetText(Text);
	}
}
