// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTTask_FindPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTTask_FindPatrolPoint(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = Speed)
	float Speed;
	
};
