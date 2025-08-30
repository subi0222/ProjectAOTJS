// Unreal Engine Marketplace Copyright


#include "BTTask_AttackTargetPlayer.h"

#include "AttackOnTitan.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanBase.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"

UBTTask_AttackTargetPlayer::UBTTask_AttackTargetPlayer(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Melee Attack TargetPlayer";
}

EBTNodeResult::Type UBTTask_AttackTargetPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAOTTitanControllerBase* TitanControllerBase  = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner());
	if (!TitanControllerBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("No Titan Controller Base"))
		return EBTNodeResult::Failed;
	}
	
	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn());
	if (!TitanBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("No Titan Base"))
		return EBTNodeResult::Failed;
	}

	TitanBase->GetCharacterMovement()->SetMovementMode(MOVE_None);
	TitanBase->TryAttack(AttackTag);
	
	//공격 Task하는 동안 Attack Montage 끝났을 때와 FinishTask(Succeeded) 바인딩
	FOnAttackEndDelegate InOnAttackEndDelegate;
	InOnAttackEndDelegate.BindLambda(
		[&]
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	
	TitanBase->SetAttackEndDelegate(InOnAttackEndDelegate);
	
	return EBTNodeResult::InProgress;
}