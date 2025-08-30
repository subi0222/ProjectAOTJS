// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TitanChooseBehavior.generated.h"

/**
 * 
 */

class AAOTTitanControllerBase;
class AAOTTitanBase;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle      UMETA(DisplayName = "Idle"),
	Patrol    UMETA(DisplayName = "Patrol"),
	Chase     UMETA(DisplayName = "Chase"),
	Escape    UMETA(DisplayName = "Escape"),
	MeleeAttack    UMETA(DisplayName = "MeleeAttack"),
	SpecialAttack    UMETA(DisplayName = "SpecialAttack"),
	Dead      UMETA(DisplayName = "Dead")
};

UCLASS()
class ATTACKONTITAN_API UBTService_TitanChooseBehavior : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_TitanChooseBehavior();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	protected:

	void UpdateTitanBehavior(UBlackboardComponent* BlackboardComp, AAOTTitanControllerBase* TitanControllerBase);

private:
	

	// bool UpdateTargetLocationTag(FGameplayTagContainer OwnedTags, AAOTTitanBase* TitanBase, float TargetZLocation);

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector MindlessTitanBehaviorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FGameplayTag MeleeAttackTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer SpecialAttackTags;
	
};	
