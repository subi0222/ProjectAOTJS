// Unreal Engine Marketplace Copyright


#include "TitanCharacter/AOTTitanBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "Components/AOTTitanCombatStateComponent.h"
#include "AttackOnTitan.h"
#include "MotionWarpingComponent.h"
#include "AOTTitanControllerBase.h"
#include "TitanCharacter/UI/Widget/AOTTitanHealthBar.h"
#include "UI/AOTTitanHPWidgetComponent.h"
#include "Components/AOTTitanHealthComponent.h"
#include "Components/AOTTitanTargetControlComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/AOTTitanStatData.h"
#include "GAS/AttributeSet/AOTTitanCombatAttributeSet.h"
#include "GAS/AttributeSet/AOTTitanHealthAttributeSet.h"
#include "GAS/GA/AOTGA_TitanAttack.h"
#include "GAS/GA/AOTGA_SeveredBodyPart.h"
#include "Perception/AIPerceptionComponent.h"
#include "UI/AOTTitanBodyTargetWidgetComponent.h"

// Sets default values
AAOTTitanBase::AAOTTitanBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Ability System
	TitanASC = CreateDefaultSubobject<UAOTTitanAbilitySystemComponent>("AbilitySystemComp");
	HealthAttributeSet = CreateDefaultSubobject<UAOTTitanHealthAttributeSet>(TEXT("HealthAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UAOTTitanCombatAttributeSet>(TEXT("CombatAttributeSet"));

	//Widget Comp
	HPWidgetComp = CreateDefaultSubobject<UAOTTitanHPWidgetComponent>("HPWidgetComp");
	HPWidgetComp->SetupAttachment(RootComponent);
	WeaknessTargetWidgetComp = CreateDefaultSubobject<UAOTTitanBodyTargetWidgetComponent>("WeaknessTargetWidgetComp");
	BodyTargetWidgetComp =CreateDefaultSubobject<UAOTTitanBodyTargetWidgetComponent>("BodyTargetWidgetComp");
	
	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpComp");
	TargetControlComp = CreateDefaultSubobject<UAOTTitanTargetControlComponent>("TargetControlComp");
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
	CombatStateComp = CreateDefaultSubobject<UAOTTitanCombatStateComponent>("CombatStateComp");
	HealthComp = CreateDefaultSubobject<UAOTTitanHealthComponent>("HealthComp");
}

void AAOTTitanBase::ApplyDataToHealthAttributeSet()
{

	// Stat 값 얻기
	float TitanHP = TitanStatData->GetHP();
	float TitanStamina = TitanStatData->GetStamina();

	TitanASC->SetNumericAttributeBase(UAOTTitanHealthAttributeSet::GetHPAttribute(), TitanHP);
	TitanASC->SetNumericAttributeBase(UAOTTitanHealthAttributeSet::GetStaminaAttribute(), TitanStamina);
}

void AAOTTitanBase::ApplyDataToCombatAttributeSet()
{
	if (TitanStatData)
	{
		float TitanDetectRadius = TitanStatData->GetDetectRadius();
		float TitanMeleeAttackDamage = TitanStatData->GetMeleeAttackDamage();
		float TitanSpecialAttackDamage = TitanStatData->GetSpecialAttackDamage();
		if (TitanASC)
		{
			TitanASC->SetNumericAttributeBase(UAOTTitanCombatAttributeSet::GetDetectRadiusAttribute(), TitanDetectRadius);
			TitanASC->SetNumericAttributeBase(UAOTTitanCombatAttributeSet::GetMeleeAttackDamageAttribute(), TitanMeleeAttackDamage);
			TitanASC->SetNumericAttributeBase(UAOTTitanCombatAttributeSet::GetSpecialAttackDamageAttribute(), TitanSpecialAttackDamage);
		}
	}
	if (TitanControllerBase)
		TitanControllerBase->UpdatePerceptionCompToTitan();
}

// Called when the game starts or when spawned
void AAOTTitanBase::BeginPlay()
{
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	//HP 위젯 세팅
	if (TitanHpWidgetClass)
	{
		HPWidgetComp->SetWidgetClass(TitanHpWidgetClass);
	}

	//ASC 세팅
	TitanASC->InitAbilityActorInfo(this, this);

	//HealthComp세팅
	HealthComp->InitializeWithAbilitySystem(TitanASC);

	//Melee Attack Ability 세팅
	if(MeleeAttackAbilityClass)
	{
		FGameplayAbilitySpec Spec(MeleeAttackAbilityClass, 1, INDEX_NONE);
		TitanASC->GiveAbility(Spec);
		if (!TitanAttackData)
		{
			AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("No Attack Data"));
			return;
		}
	}

	//Severed Body Part 세팅
	if (SeveredAbilityClass)
	{
		FGameplayAbilitySpec Spec(SeveredAbilityClass, 1, INDEX_NONE);
		TitanASC->GiveAbility(Spec);
	}

	//Passive Ability 세팅
	for (auto PassiveAbilityClass : PassiveAbilityClasses)
	{
		FGameplayAbilitySpec Spec(PassiveAbilityClass, 1, INDEX_NONE);
		TitanASC->GiveAbility(Spec);
	}
	
	if (!TitanStatData)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("No Stat Data"));
		return;
	}

	ApplyDataToCombatAttributeSet();
	ApplyDataToHealthAttributeSet();

	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("End"));
}

// Called every frame
void AAOTTitanBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAOTTitanBase::PossessedBy(AController* NewController)
{
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Begin"));
	Super::PossessedBy(NewController);

	AAOTTitanControllerBase* TC = Cast<AAOTTitanControllerBase>(NewController);
	if (!TC)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Can not Cast AIController To TitanControllerBase"))
		return;
	}

	TitanControllerBase = TC;
	if (!TitanStatData)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("No TitanStatData"))
		return;
	}
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("End"));
}

void AAOTTitanBase::PostInitializeComponents()
{
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Begin"));
	Super::PostInitializeComponents();
	HealthComp->OnTitanHealthChangedDelegate.AddDynamic(this, &AAOTTitanBase::SetHealthPercent);
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("End"));
}

void AAOTTitanBase::TryAttack(FGameplayTag AttackTag)
{
	if (!TitanASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Can Not Find AbilitySystemComponent"));
		return;
	}
	FGameplayEventData EventData;
	EventData.EventTag = AttackTag;
	EventData.Target = this;
	ensure(this != nullptr);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this, 
		AttackTag, 
		EventData);
}

UAbilitySystemComponent* AAOTTitanBase::GetAbilitySystemComponent() const
{
	return TitanASC;
}

void AAOTTitanBase::NotifyAttackEnd() const
{
	OnAttackEndDelegate.ExecuteIfBound();
	CombatStateComp->SetAttackEnd();
}

void AAOTTitanBase::SetHealthPercent(float Percent)
{
	UAOTTitanHPWidgetComponent* TmpHPWidgetComp = Cast<UAOTTitanHPWidgetComponent>(HPWidgetComp);
	if (!TmpHPWidgetComp)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("HPWidgetComp Is Null"))
		return;
	}
	TmpHPWidgetComp->Update(Percent);
}