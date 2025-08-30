// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindRandomLocation(FObjectInitializer const&  ObjectIntializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;

public:
	UPROPERTY(EditAnywhere, Category = Random)
	float RandomRadius;

	UPROPERTY(EditAnywhere, Category = Speed)
	float RunningSpeed;

	UPROPERTY(EditAnywhere, Category = Random)
	uint8 bTurning : 1;
	
};
