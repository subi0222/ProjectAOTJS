// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanTargetControlComponent.h"

#include "AttackOnTitan.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/Components/AOTTitanSeveredPartComponent.h"
#include "TitanCharacter/UI/AOTTitanBodyTargetWidgetComponent.h"

// Sets default values for this component's properties
UAOTTitanTargetControlComponent::UAOTTitanTargetControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UAOTTitanTargetControlComponent::BeginPlay()
{
	Super::BeginPlay();
	AAOTTitanBase* ATB = Cast<AAOTTitanBase>(GetOwner());
	if (!ATB)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Owner Not Yet"))
		return;
	}
	TArray<UAOTTitanSeveredPartComponent*> ISeveredParts;
	ATB->GetComponents<UAOTTitanSeveredPartComponent>(ISeveredParts);

	RegisterBodyTargets(ISeveredParts);

	//바디 타겟이 맞았을 때 ControlComponent에서 TargetControl에서 로직을 실행해줌.
	for (auto SeveredPart : ControlledSeveredParts)
	{
		SeveredPart->OnPartTargetSevered.AddDynamic(this, &UAOTTitanTargetControlComponent::SeveredBodyTarget);
		SeveredPart->OnPartTargetGotHit.AddDynamic(this, &UAOTTitanTargetControlComponent::UpdateBodyTarget);
	}

	//약점이 있을 경우도 로직을 실행해줘야 함.
	if (WeaknessSeveredPart)
	{
		WeaknessSeveredPart->OnPartTargetSevered.AddDynamic(this, &UAOTTitanTargetControlComponent::SeveredBodyTarget);
	}

	//TitanBodyTargetWidgetComp는 거인의 사지와 약점 타겟 위치를 나타내는 UI Comp임
	UAOTTitanBodyTargetWidgetComponent* BodyTargetWidgetComp =
		ATB->GetBodyTargetWidgetComponent();

	if (!BodyTargetWidgetComp)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("BodyTargetWidgetComp Is Null"))
		return;
	}

	//타겟이 바뀌었을 때, 위젯의 위치를 바꿔주는 함수 바인딩
	for (auto SeveredPart : ControlledSeveredParts)
	{
		SeveredPart->OnPartTargetStarted.AddDynamic(BodyTargetWidgetComp, &UAOTTitanBodyTargetWidgetComponent::ChangeTargetWidgetTo);
	}

	//바디 타겟 타겟팅 시작!
	if (ControlledSeveredParts.Num() > 0)
	{
		int RandIndex = FMath::RandRange(0, ControlledSeveredParts.Num() - 1);
		UAOTTitanSeveredPartComponent* RandSeveredPartComp = ControlledSeveredParts[RandIndex];
		RandSeveredPartComp->OnPartTargetStarted.Broadcast(RandSeveredPartComp);
	}
	

	//약점이 잘렸을 경우, 위젯을 지워주는 함수와 바인딩.
	UAOTTitanBodyTargetWidgetComponent* WeaknessBodyTargetWidgetComp =
		ATB->GetWeaknessWidgetComponent();
	if (!WeaknessBodyTargetWidgetComp)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("WeaknessBodyTargetWidgetComp Is Null"))
		return;
	}

	if (WeaknessSeveredPart)
	{
		WeaknessSeveredPart->OnPartTargetSevered.AddDynamic(WeaknessBodyTargetWidgetComp, &UAOTTitanBodyTargetWidgetComponent::TurnOffTargetWidget);
		WeaknessSeveredPart->OnPartTargetStarted.AddDynamic(WeaknessBodyTargetWidgetComp, &UAOTTitanBodyTargetWidgetComponent::ChangeTargetWidgetTo);
		
		//약점 Part 타겟팅 시작
		WeaknessSeveredPart->OnPartTargetStarted.Broadcast(WeaknessSeveredPart);
	}
}

//Stat Data에 있는 타겟 정보를 가져옵니다
void UAOTTitanTargetControlComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Begin"));
	AAOTTitanBase* ATB = Cast<AAOTTitanBase>(GetOwner());
	if (!ATB)
	{
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Owner Not Yet"))
		return;
	}
	TitanBase = ATB;
}


// Called every frame
void UAOTTitanTargetControlComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UAOTTitanTargetControlComponent::ChangeBodyTargetFrom(UAOTTitanSeveredPartComponent* TitanSeveredPartComponent)
{
	//Collision 없애주고
	TitanSeveredPartComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//새로운 BodyTarget을 만들어줌
	UAOTTitanSeveredPartComponent* NextBodyTargetComponent = TitanSeveredPartComponent;
	//타겟이 두 개 이상일 때는 바꿔줄 수 있음.
	if (ControlledSeveredParts.Num() > 1)
	{
		do
		{
			int RandIndex = FMath::RandRange(0, ControlledSeveredParts.Num() - 1);
			NextBodyTargetComponent = ControlledSeveredParts[RandIndex];
		}
		while (TitanSeveredPartComponent == NextBodyTargetComponent);
	}
	NextBodyTargetComponent->OnPartTargetStarted.Broadcast(NextBodyTargetComponent);
}

void UAOTTitanTargetControlComponent::RegisterBodyTargets(TArray<UAOTTitanSeveredPartComponent*> SeveredPartComponents)
{
	for (auto SeveredPartComponent : SeveredPartComponents)
	{
		//약점일 경우 항상 타겟팅이 되어야 함.
		if (SeveredPartComponent->IsWeaknessPart())
		{
			//이미 등록이 되어 있다면 약점은 하나만 있어야 한다고 말해줌.
			if (WeaknessSeveredPart)
			{
				AOT_LOG(LogAOT, Warning, TEXT("%s, Weakness Part Should be Only One!"), *SeveredPartComponent->GetName())
				continue;
			}
			//등록함.
			AOT_LOG(LogAOT, Log, TEXT("%s Weakness Registered!"), *SeveredPartComponent->GetName());
			WeaknessSeveredPart = SeveredPartComponent;
			continue;
		}
		
		//약점이 아닐 경우 Target에 추가하여 로직을 실행할 수 있도록 하고 Collision을 받지 않게 한다.
		ControlledSeveredParts.Add(SeveredPartComponent);
		SeveredPartComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AOT_LOG(LogAOT, Log, TEXT("%s Registered!"), *SeveredPartComponent->GetName());
	}
}

void UAOTTitanTargetControlComponent::UpdateBodyTarget(UAOTTitanSeveredPartComponent* SeveredPartComponent)
{
	//잘릴 때 바꿔야 한다면 그냥 냅둠!
	if (TargetChangeMode == ETargetChangeMode::ChangeTargetWhenSevered)
	{
		SeveredPartComponent->OnPartTargetStarted.Broadcast(SeveredPartComponent);
		return;
	}

	//Every Hit 마다 바꿔야 한다면!
	ChangeBodyTargetFrom(SeveredPartComponent);
}

void UAOTTitanTargetControlComponent::SeveredBodyTarget(UAOTTitanSeveredPartComponent* SeveredPartComponent)
{

	//약점이 잘렸을 경우, 일반 타겟 UI, 약점 타겟 UI, 약점 타겟과 일반 타겟들이 없어져야 함!
	if (SeveredPartComponent == WeaknessSeveredPart)
	{
		//일반 타겟들의 Collision 없애주고
		for (auto ControlledSeveredPart : ControlledSeveredParts)
			ControlledSeveredPart->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//일반 타겟 위젯 지워주고
		UAOTTitanBodyTargetWidgetComponent* BodyTargetWidgetComp = TitanBase->GetBodyTargetWidgetComponent();
		if (BodyTargetWidgetComp)
			BodyTargetWidgetComp->SetVisibility(false);

		//약점 타겟 위젯 지워주고
		UAOTTitanBodyTargetWidgetComponent* WeaknessWidgetComp = TitanBase->GetWeaknessWidgetComponent();
		if (WeaknessWidgetComp)
			WeaknessWidgetComp->SetVisibility(false);

		//일반 SeveredPart들도 지워주고
		ControlledSeveredParts.Empty();

		//약점의 Collision 없애주고
		WeaknessSeveredPart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaknessSeveredPart = nullptr;

		//약점 타겟 위젯 지워야함
		UAOTTitanBodyTargetWidgetComponent* WeaknessWidgetComponent =
			TitanBase->GetWeaknessWidgetComponent();
		if (WeaknessWidgetComponent)
			WeaknessWidgetComponent->SetVisibility(false);
	}

	//약점이 아닌 다른 타겟들이 잘렸을 경우, 해당 타겟을 지워주고 바꿔줘야 함!
	if (!ControlledSeveredParts.Contains(SeveredPartComponent))
	{
		AOT_LOG(LogAOT, Warning, TEXT("No %s"), *SeveredPartComponent->GetName());
		return;
	}

	SeveredPartComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ControlledSeveredParts.Remove(SeveredPartComponent);

	//다 잘렸을 때는 타겟이 없음
	if (ControlledSeveredParts.Num() == 0)
	{
		UAOTTitanBodyTargetWidgetComponent* BodyTargetWidgetComp = TitanBase->GetBodyTargetWidgetComponent();
		if (BodyTargetWidgetComp)
			BodyTargetWidgetComp->SetVisibility(false);
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("All Body Target Severed"));
		return;
	}

	//잘리지 않은 것들 중 아무거나 하나
	int RandIndex = FMath::RandRange(0, ControlledSeveredParts.Num() - 1);
	UAOTTitanSeveredPartComponent* NextBodyTargetComponent = ControlledSeveredParts[RandIndex];
	NextBodyTargetComponent->OnPartTargetStarted.Broadcast(NextBodyTargetComponent);
}