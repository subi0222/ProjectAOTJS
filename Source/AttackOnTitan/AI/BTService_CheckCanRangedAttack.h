// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckCanRangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTService_CheckCanRangedAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTService_CheckCanRangedAttack();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	float CalculateAngleToPlayer(ACharacter* Titan, ACharacter* Player);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "ThreshHold", meta = (AllowPrivateAccess = "True"))
	float AngleToPlayerThreshHold = 10.f;
	
};
