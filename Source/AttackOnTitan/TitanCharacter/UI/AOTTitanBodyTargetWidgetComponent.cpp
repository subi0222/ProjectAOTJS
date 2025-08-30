// Unreal Engine Marketplace Copyright


#include "TitanCharacter/UI/AOTTitanBodyTargetWidgetComponent.h"

#include "AttackOnTitan.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/Components/AOTTitanHealthComponent.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"

UAOTTitanBodyTargetWidgetComponent::UAOTTitanBodyTargetWidgetComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UAOTTitanBodyTargetWidgetComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AAOTTitanBase* TB = Cast<AAOTTitanBase>(GetOwner());
	if (!TB)
	{
		AOT_LOG(LogAOT, Warning,TEXT("%s"), TEXT("TitanBase Is Null"))
		return;
	}
	TitanBase = TB;

	UAOTTitanHealthComponent* HealthComp = TitanBase->GetComponentByClass<UAOTTitanHealthComponent>();
	if (!HealthComp)
	{
		AOT_LOG(LogAOT, Warning,TEXT("%s"), TEXT("TitanBase Is Null"))
		return;
	}

	HealthComp->OnTitanDieStartDelegate.AddDynamic(this, &UAOTTitanBodyTargetWidgetComponent::HandleTitanOnDie);
}

void UAOTTitanBodyTargetWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UUserWidget* TargetWidget = GetWidget())
	{
		// 화면 비율로 크기 조정
		TargetWidget->SetRenderScale(FVector2D(MinSize));

		// 또는 절대 픽셀 크기 유지 (Space=Screen일 때)
		// SetDrawSize(FVector2D(BaseWidth * MinSize, BaseHeight * MinSize));
	}
}

void UAOTTitanBodyTargetWidgetComponent::ChangeTargetWidgetTo(UAOTTitanSeveredPartComponent* SeveredPartComponent)
{
	AttachToComponent(SeveredPartComponent,
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SeveredPartComponent->GetAttachSocketName());
	FLinearColor Color = GetLinearColor(SeveredPartComponent);
	GetWidget()->SetColorAndOpacity(Color);
}

void UAOTTitanBodyTargetWidgetComponent::TurnOffTargetWidget(UAOTTitanSeveredPartComponent* SeveredPartComponent)
{
	SetVisibility(false);
}

void UAOTTitanBodyTargetWidgetComponent::HandleTitanOnDie()
{
	SetVisibility(false);
}

FLinearColor UAOTTitanBodyTargetWidgetComponent::GetLinearColor(UAOTTitanSeveredPartComponent* SeveredPartComp) const
{
	check(SeveredPartComp);
	float CurrentHp = SeveredPartComp->GetCurrentPartHP();
	float BaseHp = SeveredPartComp->GetPartHPBase();
	float Rate = CurrentHp / BaseHp;
	if (Rate >= HPSafeThreshold)
		return HPSafeColor;
	if (Rate >= HPWarningThreshold)
		return HPWarningColor;
	else
		return HPCriticalColor;
}


