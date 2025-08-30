// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "BTService_UpdateTitanTargetHeight.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UBTService_UpdateTitanTargetHeight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateTitanTargetHeight();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	private:
	bool UpdateTargetLocationTag(AAOTTitanBase* TitanBase, float TargetZLocation);

	FGameplayTag GetTargetTag(AAOTTitanBase* TitanBase, float TargetZLocation);

};
