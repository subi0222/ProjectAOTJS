// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionInterface.h"
#include "InteractionComponent.generated.h"

class UInteractionWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class ATTACKONTITAN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RunInteraction(bool bHit);

	// 아이템 탐지를 위한 함수
	void TraceInteraction();

	void MakeTraceParams(FCollisionQueryParams& OutTraceParams);
private:
	
	/*
	*  Interaction Properties 
	*/
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceRadius = 30.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	FVector StartOffset = FVector(0.f, 0.f, 60.f); // 캐릭터 발 위치

	/*
	*  Hit actor with Interact Interface
	*/
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	AActor* DetectedActor;

public:
	FORCEINLINE AActor* GetDetectedActor() const { return DetectedActor; }

	/*
	* Actor Array that don't require interaction
	*/ 
private:
	TArray<AActor*> IgnoreActors;
	
	// Run this function when initilizing Trace Params
	void AddActorsToIgnore(TArray<AActor*>& ActorsToIgnore);
public:
	/*
	* Add and Remove IgnoreActor Array
	*/
	void AddIgnoreActor(AActor* newActor);
	void RemoveIgnoreActor(AActor* newActor);

	/*
   * Interaction Widget
   */
protected:
	UPROPERTY(EditAnywhere)
	UInteractionWidget* InteractWidget;
public:
	void SetInteractWidget(UInteractionWidget* InWidget);

	void SetWidgetVisibiity(ESlateVisibility Visibity);

	void SetWidgetText(FText Text);
};
