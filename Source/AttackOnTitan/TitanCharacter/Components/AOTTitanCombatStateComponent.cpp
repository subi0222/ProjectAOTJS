// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanCombatStateComponent.h"


#include "AbilitySystemComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "AttackOnTitan.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "TitanCharacter/Data/AOTTitanAttackData.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanHealthAttributeSet.h"

// Sets default values for this component's properties
UAOTTitanCombatStateComponent::UAOTTitanCombatStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UAOTTitanCombatStateComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

bool UAOTTitanCombatStateComponent::HasMoreAttack() const
{
	return CurrentComboIndex < AttackInformationMap.Find(CurrentAttackTag)->NextAttackInformations.Num();
}

bool UAOTTitanCombatStateComponent::HasAttackInformation(const FGameplayTag& AttackTag)
{
	bool HasValue = AttackInformationMap.Contains(AttackTag);
	if (HasValue)
	{
		CurrentAttackTag = AttackTag;
	}
	return AttackInformationMap.Contains(AttackTag);
}

bool UAOTTitanCombatStateComponent::IsTargetCharacterInRightDistance(
	TArray<FComboAttackInformation>& ICanComboAttackInformations,
	TArray<FComboAttackInformation>& OCanComboAttackInformations)
const
{
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(TitanBase->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanController Base is NULL"));
		return false;
	}

	ACharacter* Character = TCB->GetTargetCharacter();
	if (!Character)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("There is no Target"));
		return false;
	}

	float TargetDistance = FVector::Dist2D(TitanBase->GetActorLocation(), Character->GetActorLocation());
	if (!HasMoreAttack())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("No CurrentComboIndex Is Out of NextComboInformations"));
		return false;
	}

	for (auto ComboAttackInformation : ICanComboAttackInformations)
	{
		if (ComboAttackInformation.AttackAllowedDistance - ComboAttackInformation.AttackAllowedThresholdDistance <=
			TargetDistance
			&& ComboAttackInformation.AttackAllowedDistance + ComboAttackInformation.AttackAllowedThresholdDistance >=
			TargetDistance)
		{
			OCanComboAttackInformations.Add(ComboAttackInformation);
		}
		AOT_LOG(LogAOT, Log, TEXT("Allowed Distance : %f, Target Distance : %f"),
		        ComboAttackInformation.AttackAllowedDistance, TargetDistance);
	}

	bool result = OCanComboAttackInformations.Num() > 0;
	if (!result)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("NotNearTarget!"))
	}
	return OCanComboAttackInformations.Num() > 0;
}

bool UAOTTitanCombatStateComponent::IsTargetCharacterInRightDistance(TArray<FAttackInformation>& IAttackInformations,
                                                                     TArray<FAttackInformation>&
                                                                     OInRangeAttackInformations) const
{
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(TitanBase->GetController());
	if (!TCB)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanController Base is NULL"));
		return false;
	}

	ACharacter* Character = TCB->GetTargetCharacter();
	if (!Character)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("There is no Target"));
		return false;
	}

	float TargetDistance = FVector::Dist2D(TitanBase->GetActorLocation(), Character->GetActorLocation());

	for (auto AttackInformation : IAttackInformations)
	{
		if (TargetDistance >= AttackInformation.AttackAllowedDistance - AttackInformation.AttackAllowedThresholdDistance
			&& TargetDistance <= AttackInformation.AttackAllowedDistance + AttackInformation.
			AttackAllowedThresholdDistance)
		{
			OInRangeAttackInformations.Add(AttackInformation);
		}
	}

	return OInRangeAttackInformations.Num() > 0;
}


bool UAOTTitanCombatStateComponent::HasTitanAcceptedTag(TArray<FAttackInformation>& IInRangeAttackInformations,
                                                        TArray<FAttackInformation>& OHasAcceptedTagAttackInformations)
const
{
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	if (!ASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Has No ASC"));
		return false;
	}

	for (auto AttackInformation : IInRangeAttackInformations)
	{
		bool bTargetLocationAccepted = false;
		bool bAcceptedTitanState = true;
		for (FGameplayTag RequiredTargetLocationTag : AttackInformation.TargetLocationTags)
		{
			if (ASC->HasMatchingGameplayTag(RequiredTargetLocationTag))
			{
				bTargetLocationAccepted = true;
				break;
			}
		}
		for (FGameplayTag NotAcceptedTitanStateTag : AttackInformation.NotAcceptedTitanStateTags)
		{
			if (ASC->HasMatchingGameplayTag(NotAcceptedTitanStateTag))
			{
				bAcceptedTitanState = false;
				break;
			}
		}
		if (bTargetLocationAccepted && bAcceptedTitanState)
		{
			OHasAcceptedTagAttackInformations.Add(AttackInformation);
		}
	}

	//TitanNowAllowedStateTag를 갖고 있지 않고 RequiredTargetLocationTag를 갖고 있는지 확인한다.
	return OHasAcceptedTagAttackInformations.Num() > 0;
}

bool UAOTTitanCombatStateComponent::HasTitanAcceptedTag(TArray<FComboAttackInformation>& ICanComboAttackInformations,
                                                        TArray<FComboAttackInformation>& OCanComboAttackInformations)
const
{
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	if (!ASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Has No ASC"));
		return false;
	}
	//콤보 공격이 아닐 경우
	if (CurrentComboIndex < 0)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Can Not Combo"))
		return false;
	}

	//매개변수로 통과된 ComboAttackInformation 중에 공격할 수 없는 태그가 있을 경우!
	for (FComboAttackInformation ComboAttackInformation : ICanComboAttackInformations)
	{
		bool bAccepted = true;
		//공격할 수 없는 상태의 태그
		for (FGameplayTag TitanNotAllowedStateTag : ComboAttackInformation.NotAcceptedTitanStateTags)
		{
			if (ASC->HasMatchingGameplayTag(TitanNotAllowedStateTag))
			{
				bAccepted = false;
				break;
			}
		}
		if (bAccepted)
		{
			OCanComboAttackInformations.Add(ComboAttackInformation);
		}
	}
	return OCanComboAttackInformations.Num() > 0;
}

bool UAOTTitanCombatStateComponent::HasEnoughStamina(TArray<FAttackInformation>& IHasAcceptedTagAttackInformations)
{
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	float NowStamina = ASC->GetSet<UAOTTitanHealthAttributeSet>()->GetStamina();

	for (auto AttackInformation : IHasAcceptedTagAttackInformations)
	{
		if (NowStamina >= AttackInformation.AttackStaminaCost)
		{
			CanAttackInformations.Add(AttackInformation);
		}
	}

	return CanAttackInformations.Num() > 0;
}

bool UAOTTitanCombatStateComponent::HasEnoughStamina(
	TArray<FComboAttackInformation>& IHasAcceptedTagAttackInformations)
{
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	float NowStamina = ASC->GetSet<UAOTTitanHealthAttributeSet>()->GetStamina();

	for (auto AttackInformation : IHasAcceptedTagAttackInformations)
	{
		if (NowStamina >= AttackInformation.AttackStaminaCost)
		{
			CanNextComboAttackInformations.Add(AttackInformation);
		}
	}

	return CanNextComboAttackInformations.Num() > 0;
}

void UAOTTitanCombatStateComponent::SetCurrentComboExactIndex(int ComboIndex,
                                                              FComboAttackInformation& ComboAttackInformation)
{
	if (ComboIndex < 0 || ComboIndex >= CurrentAttackInformation.NextAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Can Not Combo"));
		CurrentComboExactIndex = -1;
		return;
	}

	for (int i = 0; i < CurrentAttackInformation.NextAttackInformations[ComboIndex].ComboAttackInformations.Num(); i++)
	{
		if (ComboAttackInformation == CurrentAttackInformation.NextAttackInformations[ComboIndex].
			ComboAttackInformations[i])
		{
			CurrentComboIndex = i;
			return;
		}
	}
}


// Called every frame
void UAOTTitanCombatStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAOTTitanCombatStateComponent::InitializeComponent()
{
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Begin"));
	Super::InitializeComponent();
	TitanBase = Cast<AAOTTitanBase>(GetOwner());
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Cannot Cast TitanBase"));
		return;
	}

	UAOTTitanAttackData* AttackData = TitanBase->GetTitanAttackData();
	if (!AttackData)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanBase Does Not Have AttackData"));
		return;
	}

	for (auto AttackInformation : AttackData->GetAttackInformations())
	{
		if (AttackInformation.AttackTag == FGameplayTag::EmptyTag)
		{
			AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Not Assigned Attack Tag"))
		}
		AttackInformationMap.Add({AttackInformation.AttackTag, AttackInformation});
	}
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("End"));
}

//첫 공격을 시작할 수 있는지 봄
bool UAOTTitanCombatStateComponent::CanAttackStart(FGameplayTag& AttackTag)
{
	 bEnoughStamina = true;
	//Key에 해당하는 Map Value가 있는지 확인
	if (!HasAttackInformation(AttackTag))
	{
		AOT_LOG(LogAOT, Log, TEXT("There is no value that has %s key"), *AttackTag.ToString())
		return false;
	}

	//AttackInformation 찾고 CombatState 초기화
	TArray<FAttackInformation> RightTagAttackInformations;
	AttackInformationMap.MultiFind(AttackTag, RightTagAttackInformations);

	//타겟이 가까이에 있는지 확인
	TArray<FAttackInformation> InRangeAttackInformations;
	if (!IsTargetCharacterInRightDistance(RightTagAttackInformations, InRangeAttackInformations))
	{
		return false;
	}

	//Titan이 갖고 있는 Gameplay Tag 확인(TargetLocationTag, TitanStateTag)
	TArray<FAttackInformation> HasAcceptedTagAttackInformations;
	if (!HasTitanAcceptedTag(InRangeAttackInformations, HasAcceptedTagAttackInformations))
	{
		return false;
	}
	
	//마지막으로 해당 공격을 할 수 있는 충분한 스태미나가 있는지 확인
	if (HasEnoughStamina(HasAcceptedTagAttackInformations))
	{
		int32 RandomIndex = FMath::RandRange(0, CanAttackInformations.Num() - 1);
		CurrentAttackInformation = CanAttackInformations[RandomIndex];
		CanNextComboAttackInformations.Empty();
		CurrentComboIndex = -1;
		CurrentComboExactIndex = -1;
		NextComboSectionName.Empty();
		return true;
	}
	
	bEnoughStamina = false;
	return false;
}

bool UAOTTitanCombatStateComponent::CanComboAttack()
{
	//콤보 인덱스 올리고
	CurrentComboIndex++;

	//이미 남아있는 공격이 없다면 Out
	if (!HasMoreAttack())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Out of Index"))
		return false;
	}

	TArray<FComboAttackInformation> ComboAttackInformations = CurrentAttackInformation.NextAttackInformations[
			CurrentComboIndex]
		.ComboAttackInformations;

	TArray<FComboAttackInformation> NearComboAttackInformations;

	//타겟이 공격 반경 거리에 맞게 있는지 확인 
	if (!IsTargetCharacterInRightDistance(ComboAttackInformations, NearComboAttackInformations))
	{
		return false;
	}

	//현재 Titan의 ASC 태그가 콤보 공격에 알맞게 있는지 확인
	TArray<FComboAttackInformation> OAcceptedComboAttackInformations;
	if (!HasTitanAcceptedTag(NearComboAttackInformations, OAcceptedComboAttackInformations))
	{
		return false;
	}

	//현재 Titan의 스태미너 양이 충분히 있는지 확인
	if (HasEnoughStamina(OAcceptedComboAttackInformations))
	{
		int32 RandomIndex = FMath::RandRange(0, CanNextComboAttackInformations.Num() - 1);
		bEnoughStamina = true;
		CurrentComboAttackInformation = CanNextComboAttackInformations[RandomIndex];
		CanNextComboAttackInformations.Empty();
		NextComboSectionName = CurrentComboAttackInformation.MontageSectionName;
		return true;
	}
	return false;
}

bool UAOTTitanCombatStateComponent::CanRotate()
{
	//아직 CurrentAttackModeKey가 초기화되어 있지 않을 경우
	if (!AttackInformationMap.Contains(CurrentAttackTag))
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Can Not Know CurrentAttackInformation"));
		return false;
	}
	//첫 공격이 시작되었을 경우
	if (CurrentComboIndex == -1)
	{
		return CurrentAttackInformation.bRotating;
	}

	//
	//맞지 않은 값이 들어왔을 때
	if (CurrentComboIndex >= CurrentAttackInformation.NextAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Current Combo Index Is Out Of NextAttackInformations"))
		return false;
	}

	if (CurrentComboExactIndex < 0 && CurrentComboExactIndex >=
		CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Next Combo Index Is Out Of ComboAttackInformations"
			        "In NextComboAttackInformation"))
		return false;
	}

	//정확한 값이 들어왔을 경우, 
	return CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations[CurrentComboExactIndex].bRotating;
}

void UAOTTitanCombatStateComponent::JumpToNextCombo()
{
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Titan Is Nullptr"));
		return;
	}
	UAnimInstance* TitanAnimInstance = TitanBase->GetMesh()->GetAnimInstance();
	if (!TitanAnimInstance)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("AnimInstance Is Nullptr"));
		return;
	}
	AOT_LOG(LogAOT, Log, TEXT("Section Name : %s"), *NextComboSectionName);
	TitanAnimInstance->Montage_JumpToSection(*NextComboSectionName);
}

void UAOTTitanCombatStateComponent::SetAttackTag(FGameplayTag AttackTag)
{
	CurrentAttackTag = AttackTag;
}

//기본적으로 문제가 생겼다면 -1을 반환합니다.
float UAOTTitanCombatStateComponent::GetRequiredStamina()
{
	//아직 CurrentAttackModeKey가 초기화되어 있지 않을 경우
	if (!AttackInformationMap.Contains(CurrentAttackTag))
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Can Not Know CurrentAttackInformation"));
		return -1.0f;
	}
	//첫 공격이 시작되었을 경우
	if (CurrentComboIndex == -1)
	{
		return CurrentAttackInformation.AttackStaminaCost;
	}

	//
	//맞지 않은 값이 들어왔을 때
	if (CurrentComboIndex >= CurrentAttackInformation.NextAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Current Combo Index Is Out Of NextAttackInformations"))
		return -1;
	}

	if (CurrentComboExactIndex < 0 && CurrentComboExactIndex >=
		CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Next Combo Index Is Out Of ComboAttackInformations"
			        "In NextComboAttackInformation"))
		return -1.f;
	}

	//정확한 값이 들어왔을 경우, 
	return CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations[CurrentComboExactIndex].AttackStaminaCost;
}

bool UAOTTitanCombatStateComponent::IsAnyAttackInRange(ACharacter* Character)
{
	if (!Character)
	{
		AOT_LOG(LogAOTAI, Log, TEXT("%s"), TEXT("Target Not Yet"))
		return false;
	}
	for (auto AttackInformationTuple : AttackInformationMap)
	{
		FAttackInformation AttackInfo = AttackInformationTuple.Value;
		AOT_LOG(LogAOTAI, Log, TEXT("Titan To Target Distance : %f, Attack Allowed Distance : %f"),
		        TitanBase->GetDistanceTo(Character), AttackInfo.AttackAllowedDistance);
		bool IsInRange = TitanBase->GetDistanceTo(Character) <= AttackInfo.AttackAllowedDistance;
		if (IsInRange)
		{
			return true;
		}
	}
	return false;
}

void UAOTTitanCombatStateComponent::SetAttackEnd()
{
	CanAttackInformations.Empty();
	CanNextComboAttackInformations.Empty();
	CurrentComboIndex = -1;
	CurrentComboExactIndex = -1;
	CurrentAttackTag = FGameplayTag::EmptyTag;
	CurrentAttackInformation = FAttackInformation();
	CurrentComboAttackInformation = FComboAttackInformation();
	NextComboSectionName.Empty();
}

bool UAOTTitanCombatStateComponent::CanPerformAnyAttack()
{
	if (!bCanPerformAttack)
	{
		return false;
	}
	
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	if (!ASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Titan Has No ASC"));
		return false;
	}
	
	for (auto AttackInformationTuple : AttackInformationMap)
	{
		FAttackInformation AttackInformation = AttackInformationTuple.Value;
		bool bCanAttack = true;
		for (FGameplayTag NotAcceptedTag : AttackInformation.NotAcceptedTitanStateTags)
		{
			if (ASC->HasMatchingGameplayTag(NotAcceptedTag))
			{
				bCanAttack = false;
				break;
			}
		}
		if (bCanAttack)
			return true;
	}

	bCanPerformAttack = false;
	return false;
}

TArray<FSpawnActorInformation> UAOTTitanCombatStateComponent::GetCurrentAttackSpawnActorInformations()
{
	//아직 CurrentAttackModeKey가 초기화되어 있지 않을 경우
	if (!AttackInformationMap.Contains(CurrentAttackTag))
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Can Not Know CurrentAttackInformation"));
		return TArray<FSpawnActorInformation>();
	}
	//첫 공격이 시작되었을 경우
	if (CurrentComboIndex == -1)
	{
		return CurrentAttackInformation.SpawnActorInformations;
	}

	//
	//맞지 않은 값이 들어왔을 때
	if (CurrentComboIndex >= CurrentAttackInformation.NextAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Current Combo Index Is Out Of NextAttackInformations"))
		return TArray<FSpawnActorInformation>();
	}

	if (CurrentComboExactIndex < 0 && CurrentComboExactIndex >=
		CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations.Num())
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Next Combo Index Is Out Of ComboAttackInformations"
			        "In NextComboAttackInformation"))
		return TArray<FSpawnActorInformation>();
	}

	//정확한 값이 들어왔을 경우, 
	return CurrentAttackInformation.NextAttackInformations[CurrentComboIndex]
		.ComboAttackInformations[CurrentComboExactIndex].SpawnActorInformations;
}
