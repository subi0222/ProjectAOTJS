// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TitanCharacter/AOTTitanBase.h"


UAOTTitanSeveredPartComponent::UAOTTitanSeveredPartComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UAOTTitanSeveredPartComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAOTTitanSeveredPartComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AAOTTitanBase* TB = Cast<AAOTTitanBase>(GetOwner());
	if (!TB)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Titan Base Is Null"));
		return;
	}
	TitanBase = TB;
	CurrentPartHP = PartHPBase;
	OnPartTargetStarted.AddDynamic(this, &UAOTTitanSeveredPartComponent::InitSeveredPart);
}

void UAOTTitanSeveredPartComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("Titan Base Is Null"))
		return;
	}
}

UAnimMontage* UAOTTitanSeveredPartComponent::GetRandomSeveredMontage() const
{
	if (SeveredPlayMontages.Num() == 0)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("SeveredPlayMontage Is Null"))
		return nullptr;
	}
	int RandIndex = FMath::RandRange(0, SeveredPlayMontages.Num() - 1);
	return SeveredPlayMontages[RandIndex];
}

UAnimMontage* UAOTTitanSeveredPartComponent::GetRandomStateTransitionMontage() const
{
	if (StateTransitionMontages.Num() == 0)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("StateTransitionMontage Is Null"))
		return nullptr;
	}
	int RandIndex = FMath::RandRange(0, StateTransitionMontages.Num());
	return StateTransitionMontages[RandIndex];
}

void UAOTTitanSeveredPartComponent::GotHit(float Damage)
{
	//현재 남아있는 HP 계산
	CurrentPartHP = FMath::Clamp(CurrentPartHP - Damage, 0.0f, PartHPBase);
	if (CurrentPartHP <= 0.0f)
	{
		TriggerSeveredEvent();
		OnPartTargetSevered.Broadcast(this);
		return;
	}
	OnPartTargetGotHit.Broadcast(this);
}

void UAOTTitanSeveredPartComponent::TriggerSeveredEvent()
{
	UAbilitySystemComponent* TitanASC = TitanBase->GetAbilitySystemComponent();
	if (!TitanASC)
	{
		AOT_LOG(LogAOT,Warning, TEXT("%s"), TEXT("Titan ASC Is Null"));
		return;
	}
	FGameplayEventData EventData;
	EventData.EventTag = Titan_Event_Severed;
	EventData.OptionalObject = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TitanBase, Titan_Event_Severed, EventData);
}

void UAOTTitanSeveredPartComponent::InitSeveredPart(UAOTTitanSeveredPartComponent* TitanSeveredPartComponent)
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
