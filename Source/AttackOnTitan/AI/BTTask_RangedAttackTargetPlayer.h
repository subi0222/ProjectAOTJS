// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangedAttackTargetPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTTask_RangedAttackTargetPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTTask_RangedAttackTargetPlayer(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> SkillTags;

private:

	int CurrentSkillIndex = 0;

	
};
