// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AOTTitanRotatingComponent.generated.h"


class AAOTTitanControllerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UAOTTitanRotatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAOTTitanRotatingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	FRotator GetNextRotator(float DeltaTime);

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	TObjectPtr<AAOTTitanControllerBase> TitanController;

		
};
