// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackTargetPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTTask_AttackTargetPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTTask_AttackTargetPlayer(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FGameplayTag AttackTag;
	
};
