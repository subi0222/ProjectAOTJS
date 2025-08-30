// Unreal Engine Marketplace Copyright


#include "AI/BTService_CheckCanRangedAttack.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_CheckCanRangedAttack::UBTService_CheckCanRangedAttack()
{
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	NodeName = "CheckTitanCanRangedAttack";
}

void UBTService_CheckCanRangedAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (auto* const TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner())) {
		if (auto* const TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn())) {
			if (auto BlackBoardComp = OwnerComp.GetBlackboardComponent()) {
				float AngleToPlayer = CalculateAngleToPlayer(TitanBase, TitanControllerBase->GetTargetCharacter());
				bool IsAttacking = TitanBase->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
				bool CanSpecialAttack =  AngleToPlayer <= AngleToPlayerThreshHold;
				BlackBoardComp->SetValueAsBool("CanSpecialAttack", CanSpecialAttack);
				BlackBoardComp->SetValueAsBool("IsAttacking", IsAttacking);
			}
		}
	}
}

float UBTService_CheckCanRangedAttack::CalculateAngleToPlayer(ACharacter* Titan, ACharacter* Player)
{
	if (!Player)
		return 180.f;
	
	FVector TitanLocation = Titan->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();

	FRotator TitanRotation = Titan->GetActorRotation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TitanLocation, PlayerLocation);

	// Yaw �� ���� ���
	float AngleDifference = FMath::Abs(TitanRotation.Yaw - LookAtRotation.Yaw);
	// ���� ���� (0~180�� ���� ����)
	AngleDifference = FMath::Fmod(AngleDifference + 180.0f, 360.0f) - 180.0f;

	return AngleDifference;
}
