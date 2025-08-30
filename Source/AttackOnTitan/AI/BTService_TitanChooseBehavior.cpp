// Unreal Engine Marketplace Copyright


#include "BTService_TitanChooseBehavior.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "TitanCharacter/Components/AOTTitanCombatStateComponent.h"


UBTService_TitanChooseBehavior::UBTService_TitanChooseBehavior()
{
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	NodeName = "ChooseBehavior";
}

void UBTService_TitanChooseBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	//
	// ATitanControllerBase* TCB = Cast<ATitanControllerBase>(OwnerComp.GetAIOwner());
	// if (!TCB)
	// {
	// 	AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Controller Base Is Nullptr"))
	// 	return;
	// }
	//
	// AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TCB->GetPawn());
	// if (!TitanBase)
	// {
	// 	AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Base Is Nullptr"))
	// 	return;
	// }
	//
	// UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	// if (!BlackboardComponent)
	// {
	// 	AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Blackboard Component Is Nullptr"))
	// 	return;
	// }
	//
	// UpdateTitanBehavior(BlackboardComponent, TCB);
	//
}

void UBTService_TitanChooseBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner());
	if (!TCB)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Controller Base Is Nullptr"))
		return;
	}

	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TCB->GetPawn());
	if (!TitanBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Base Is Nullptr"))
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Blackboard Component Is Nullptr"))
		return;
	}

	UpdateTitanBehavior(BlackboardComponent, TCB);
}

void UBTService_TitanChooseBehavior::UpdateTitanBehavior(UBlackboardComponent* BlackboardComp,
                                                         AAOTTitanControllerBase* TitanControllerBase)
{
	TitanControllerBase->ToggleUpdateTarget(true);
	TitanControllerBase->UpdateTarget();

	//현재 타겟이 없는 경우 Patrol Or Idle
	if (!TitanControllerBase->HasTarget())
	{
		//시선이 플레이어로 향하면 안됨
		TitanControllerBase->ToggleUpdateTarget(false);
		if (TitanControllerBase->HasPath())
		{
			BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName, static_cast<uint8>(EAIState::Patrol));
			return;
		}

		// TO DO : Patrol Point이던지, 가만히 있는 Idle인지 !
		BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName, static_cast<uint8>(EAIState::Idle));
		return;
	}

	// TitanControllerBase->ToggleTurningToTarget(true);

	//타겟이 있을 경우
	//공격을 하고 있을 때는 플레이어에게 시선이 가지 않고 행동을 고르지 말아야 함.
	if (TitanControllerBase->IsAttacking())
	{
		return;
	}
	

	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn());
	if (!TitanBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Base Is Null"))
		return;
	}

	UAOTTitanCombatStateComponent* CombatStateComp = TitanBase->FindComponentByClass<UAOTTitanCombatStateComponent>();
	if (!CombatStateComp)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Combat State Component Is Null"))
		return;
	}

	//공격을 할 수 없는 상태라면 아예 플레이어로부터 도망친다.
	if (!CombatStateComp->CanPerformAnyAttack())
	{
		TitanControllerBase->ToggleActivateRotatingComp(false);
		BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName,
							   static_cast<uint8>(EAIState::Escape));
		return;
	}

	bool bCanMeleeAttack = CombatStateComp->CanAttackStart(MeleeAttackTag);
	if (bCanMeleeAttack)
	{
		BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName,
									   static_cast<uint8>(EAIState::MeleeAttack));
		return;
	}
	
	if (SpecialAttackTags.Num() > 0)
	{
		bool bCanSpecialAttack = false;
		for (FGameplayTag SpecialAttackTag : SpecialAttackTags)
		{
			bCanSpecialAttack = CombatStateComp->CanAttackStart(SpecialAttackTag);
			if (bCanSpecialAttack)
			{
				BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName,
					static_cast<uint8>(EAIState::SpecialAttack));
				return;
			}
		}
	}

	TitanControllerBase->ToggleActivateRotatingComp(true);
	BlackboardComp->SetValueAsEnum(MindlessTitanBehaviorKey.SelectedKeyName, static_cast<uint8>(EAIState::Chase));
}
