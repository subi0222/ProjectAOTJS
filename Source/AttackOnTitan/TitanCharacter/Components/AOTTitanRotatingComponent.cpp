// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanRotatingComponent.h"

#include "AttackOnTitan.h"
#include "Kismet/KismetMathLibrary.h"
#include "TitanCharacter/AOTTitanControllerBase.h"

// Sets default values for this component's properties
UAOTTitanRotatingComponent::UAOTTitanRotatingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UAOTTitanRotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAOTTitanRotatingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(GetOwner());
	if (!TCB)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("Titan Controller Base Is Nullptr"))
		return;
	}
	TitanController = TCB;
}


FRotator UAOTTitanRotatingComponent::GetNextRotator(float DeltaTime)
{
	if (!TitanController)
	{
		return GetOwner()->GetActorForwardVector().Rotation();
	}

	APawn* Pawn = TitanController->GetPawn();
	if (!Pawn)
	{
		return GetOwner()->GetActorForwardVector().Rotation();
	}

	ACharacter* TargetChar = TitanController->GetTargetCharacter();
	if (!TargetChar)
	{
		return GetOwner()->GetActorForwardVector().Rotation();
	}

	const FRotator Current = Pawn->GetActorRotation();

	// 목표: 내 위치 -> 타깃 위치를 바라보는 회전
	FRotator Target = UKismetMathLibrary::FindLookAtRotation(
		Pawn->GetActorLocation(),
		TargetChar->GetActorLocation()
	);

	// 수평만 회전(필요 시)
	Target.Pitch = 0.f;
	Target.Roll  = 0.f;

	// 부드럽게 근접(지수 감쇠형)
	const float TurnSpeed = TitanController->GetTurningSpeed(); // 보간 속도(InterpSpeed)
	const FRotator NewRot = FMath::RInterpTo(Current, Target, DeltaTime, TurnSpeed);

	return NewRot; // 절대 회전값 반환
}

void UAOTTitanRotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TitanController)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Titan Controller base is nullptr"));
		return;
	}

	APawn* Pawn = TitanController->GetPawn();
	if (!Pawn)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("Pawn is nullptr"));
		return;
	}

	const FRotator NewRotator = GetNextRotator(DeltaTime);

	// 절대 회전값이므로 SetActorRotation 사용
	Pawn->SetActorRotation(NewRotator);
}