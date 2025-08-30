// Unreal Engine Marketplace Copyright


#include "AOTTitanControllerBase.h"

#include "AttackOnTitan.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/AOTTitanHealthComponent.h"
#include "Components/AOTTitanRotatingComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"

void AAOTTitanControllerBase::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	auto* const Target = Cast<ACharacter>(Actor);
	if (Target && Target->IsPlayerControlled()) {

		// 시야에서 보일 경우 TargetList에 추가해준다.
		bool bCanDetecPlayer = Stimulus.WasSuccessfullySensed();
		if (bCanDetecPlayer)
		{
			TargetList.Add(Target);
			return;
		}

		// 시야에서 사라졌을 경우 TargetList에서 없앤다.
		check(!bCanDetecPlayer);
		TargetList.Remove(Target);
	}
}

void AAOTTitanControllerBase::SetupAIConfig()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	
	if (SightConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAOTTitanControllerBase::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	if (DamageConfig)
	{
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}
}

void AAOTTitanControllerBase::UpdateTarget()
{
	//타겟을 업데이트 하지 않을 경우 return
	if (!bUpdateTarget)
		return;
	
	//Target이 없을 경우 업데이트를 하지 않는다.
	if (TargetList.Num() <= 0)
		return;

	ACharacter* NewTarget = Cast<ACharacter>(TargetList.Last());
	float Distance = GetPawn()->GetDistanceTo(NewTarget);
	for (ACharacter* Target : TargetList)
	{
		float TempDistance = GetPawn()->GetDistanceTo(Target);
		if (TempDistance < Distance)
		{
			NewTarget = Target;
			Distance = TempDistance;
		}
	}
	if (CurrentTargetCharacter != NewTarget)
		CurrentTargetCharacter = NewTarget;
	
}

void AAOTTitanControllerBase::UpdatePerceptionCompToTitan()
{
	if (TitanBase)
	{
		float DetectRadius = TitanBase->GetAbilitySystemComponent()->GetSet<UAOTTitanCombatAttributeSet>()->GetDetectRadius(); 
		SightConfig->SightRadius = DetectRadius;
		GetPerceptionComponent()->RequestStimuliListenerUpdate();
	}
}

AAOTTitanControllerBase::AAOTTitanControllerBase(FObjectInitializer const& ObjectInitializer)
{
	SetupAIConfig();
	RotatingComp = CreateDefaultSubobject<UAOTTitanRotatingComponent>("RotatingComp");
}

void AAOTTitanControllerBase::SetAttacking(bool bActivate)
{
	bAttacking = bActivate;
	ToggleActivateRotatingComp(bActivate);
}

void AAOTTitanControllerBase::BeginPlay()
{
	Super::BeginPlay();
	RotatingComp->SetComponentTickEnabled(false);
}

float AAOTTitanControllerBase::GetDistanceToPlayer()
{
	return FVector2D::Distance(FVector2D(GetPawn()->GetActorLocation()), FVector2D(CurrentTargetCharacter->GetActorLocation()));
}

void AAOTTitanControllerBase::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	TitanBase = Cast<AAOTTitanBase>(NewPawn);
	if (!TitanBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("New Pawn Can Not Cast To AOTTitanBase"));
		return;
	}

	//죽기 시작했을 때, 자동으로 controller가 없어지도록
	UAOTTitanHealthComponent* HealthComp = TitanBase->GetComponentByClass<UAOTTitanHealthComponent>();
	if (!HealthComp)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("HealthComp Is Nullptr"));
		return;
	}
	
	HealthComp->OnTitanDieStartDelegate.AddDynamic(this, &AAOTTitanControllerBase::UnPossess);
	
	if (BehaviorTreeAsset)
	{
		if (!BehaviorTreeAsset->BlackboardAsset)
		{
			AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("BehaviorTreeAsset->BlackboardAsset Is Null"));
			return;
		}
		UBlackboardComponent* BlackboardComponent;
		UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent);
		Blackboard = BlackboardComponent;
		RunBehaviorTree(BehaviorTreeAsset);
		SightConfig->SightRadius = 720.f;
		GetPerceptionComponent()->RequestStimuliListenerUpdate();
	}
	
	AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("BehaviorTreeAsset Is Not In Blueprint"));
}
