// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EscapeFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTTask_EscapeFromPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


public:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetLocation;
	
	UPROPERTY(EditAnywhere)
	float EscapeDistance;

	UPROPERTY(EditAnywhere)
	float EscapeSpeed;
	
};
