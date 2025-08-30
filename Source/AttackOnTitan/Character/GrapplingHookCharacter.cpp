// Unreal Engine Marketplace Copyright


#include "GrapplingHookCharacter.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CableComponent.h"

AGrapplingHookCharacter::AGrapplingHookCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BoostFadeInComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("BoostFadeIn"));
    BoostFadeOutComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("BoostFadeOut"));
    SwingFadeInComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("SwingFadeIn"));
    RotateFadeComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotateFade"));
	EndBoostFadeComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("EndBoostFade"));
	AirDodgeRotationComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("AidDodgeRotationOverride"));

	// ī�޶� ���� 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	CableLeft = CreateDefaultSubobject<UCableComponent>(TEXT("CableLeft"));
	CableLeft->SetupAttachment(RootComponent);
	CableRight = CreateDefaultSubobject<UCableComponent>(TEXT("CableRight"));
	CableRight->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AGrapplingHookCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnBeginHooked.AddDynamic(this, &AGrapplingHookCharacter::EventOnBeginHooked);
	OnBeginBoost.AddDynamic(this, &AGrapplingHookCharacter::EventOnBeginBoost);
	OnEndBoost.AddDynamic(this, &AGrapplingHookCharacter::EventOnEndBoost);
	NoLongerHooked.AddDynamic(this, &AGrapplingHookCharacter::EventNoLongerHooked);
	OnReceiveHookLocation.AddDynamic(this, &AGrapplingHookCharacter::EventOnReceiveHookLocation);
}

void AGrapplingHookCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckforHookBlock(true);
	CheckforHookBlock(false);
	CaculateAndAddInAirForce();
	AdjustCamera();
	SpawnGroundVisualEffects();
	SetHookVisibility();
	CalculateVelocityDotHook();
	ClampMaxVelocity();
	CalculateSmartAimTargetLocations();
	//DisplayRope();
}

void AGrapplingHookCharacter::EventOnBeginHooked()
{
	 GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AGrapplingHookCharacter::EventOnBeginBoost()
{
	if (BoostFadeInCurve && BoostFadeInComponent)
	{
		BoostFadeInTrack.BindDynamic(this, &AGrapplingHookCharacter::BoostFadeInProgress);

		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindDynamic(this, &AGrapplingHookCharacter::BoostFadeInFinished);

		BoostFadeInComponent->AddInterpFloat(BoostFadeInCurve, BoostFadeInTrack);
		BoostFadeInComponent->SetTimelineFinishedFunc(FinishedFunction);
		BoostFadeInComponent->PlayFromStart();
	}
}

void AGrapplingHookCharacter::BoostFadeInProgress(float Value)
{
	if (bIsBoosting)
	{
		BoostForceMultiplier = Value;
		InBoostFade = true;
	}
}

void AGrapplingHookCharacter::BoostFadeInFinished()
{
	InBoostFade = false;
}

void AGrapplingHookCharacter::EventOnEndBoost()
{
	if (BoostFadeOutCurve && BoostFadeOutComponent && BoostFadeInComponent)
	{
		BoostFadeOutTrack.BindDynamic(this, &AGrapplingHookCharacter::BoostFadeOutProgress);

		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindDynamic(this, &AGrapplingHookCharacter::BoostFadeOutFinished);

		BoostFadeInComponent->Stop();
		BoostFadeOutComponent->AddInterpFloat(BoostFadeOutCurve, BoostFadeOutTrack);
		BoostFadeInComponent->SetTimelineFinishedFunc(FinishedFunction);
		BoostFadeOutComponent->PlayFromStart();
	}

	if (bRightHookAttached || bLeftHookAttached)
	{
		SwingFadeInTrack.BindDynamic(this, &AGrapplingHookCharacter::SwingFadeInProgress);

		if (SwingFadeInCurve && SwingFadeInComponent && BoostFadeInComponent)
		{
			SwingFadeInComponent->AddInterpFloat(SwingFadeInCurve, SwingFadeInTrack);
			SwingFadeInComponent->PlayFromStart();
		}
	}
}

void AGrapplingHookCharacter::BoostFadeOutProgress(float Value)
{
	if (!bIsBoosting)
	{
		BoostForceMultiplier = Value;
		InBoostFade = true;
		// Clamp Velocity Max Size
		double MaxSize = FMath::Lerp(MaxAirVelocity, MaxAirBoostVelocity, Value);
		FVector ClampedVelocity = GetCharacterMovement()->Velocity.GetClampedToSize(0.f, MaxSize);
		GetCharacterMovement()->Velocity = ClampedVelocity;
	}
}

void AGrapplingHookCharacter::SwingFadeInProgress(float Value)
{
	SwingForceMultiplier = Value;
}

void AGrapplingHookCharacter::EndBoostFadeProgress(float Value)
{
	if (!bIsBoosting)
	{
		GetCharacterMovement()->GroundFriction = UKismetMathLibrary::Lerp(GroundFriction, GetCharacterMovement()->GroundFriction, Value);
		GetCharacterMovement()->BrakingDecelerationWalking = UKismetMathLibrary::Lerp(BrakingDecelerationWalking, GetCharacterMovement()->BrakingDecelerationWalking, Value);
		GetCharacterMovement()->MaxAcceleration = UKismetMathLibrary::Lerp(MaxAcceleration, GetCharacterMovement()->MaxAcceleration, Value);
	}
}

void AGrapplingHookCharacter::EndBoostFadeFinished()
{
	SetIsBoosting(false);
}

void AGrapplingHookCharacter::AirDodgeRotationProgress(float Value)
{
	FRotator SelectedRot = FRotator();
	if (!(bRightHookAttached || bLeftHookAttached) && bIsBoosting)
	{
		SelectedRot = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
	}
	else
	{
		SelectedRot = HookedDesiredRotation;
	}

	if (AirDodgeDirection == EDirection::Left || AirDodgeDirection == EDirection::Right)
	{
		SelectedRot = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
	}
	SetActorRotation(UKismetMathLibrary::RLerp(TempRotation, SelectedRot, Value, false));
}

void AGrapplingHookCharacter::AirDodgeRotationFinished()
{
	bRotationCalculationOverride = false;
}

void AGrapplingHookCharacter::CheckNoLongerHooked()
{
	if (!(bRightHookAttached || bLeftHookAttached))
	{
		NoLongerHooked.Broadcast();
	}
	else
	{
		OnBeginHooked.Broadcast();
	}
}

void AGrapplingHookCharacter::FireHookProjectile(bool IsRight)
{
	if (IsRight)
	{
		bIsRightLocal = true;
		bWantsRightHook = true;
	}
	else
	{
		bIsRightLocal = false;
		bWantsLeftHook = true;
	}
	if (bIsRightLocal? !IsValid(RightHookProjectile) : !IsValid(LeftHookProjectile))
	{
		FVector SmartAimLocation = bIsRightLocal ? SmartAimLocationRight : SmartAimLocationLeft;
		FVector Start = FollowCamera->GetComponentLocation();
		FVector End = FVector();
		if (bIsSmartAiming && SmartAimLocation != FVector())
		{
			End = SmartAimLocation;
		}
		else
		{
			End = FollowCamera->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(GetBaseAimRotation()) * 100000.0;
		}

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
			CollisionParams);

		FVector TargetLocation = FVector();
		if (HitResult.bBlockingHit)
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		else
		{
			TargetLocation = HitResult.TraceEnd;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		//AActor* HookProjectile = GetWorld()->SpawnActor<AActor>(HookProjectileClass,
		//	GetActorLocation(),
		//	UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation),
		//	SpawnParams
		//);

		//bIsRightLocal ? RightHookProjectile = HookProjectile : LeftHookProjectile = HookProjectile;

		HookProjectileFire(IsRight, GetActorLocation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation));}
}



void AGrapplingHookCharacter::HookProjectileFire_Implementation(bool IsRight, FVector SpawnLocation, FRotator SpawnRotation)
{
}

void AGrapplingHookCharacter::ReleaseHookProjectile(bool IsRight)
{
	IsRight ? bWantsRightHook = false : bWantsLeftHook = false;
	IsRight ? RightHookPoints.Empty() : LeftHookPoints.Empty();
	IsRight ? RightHookPointNormals.Empty() : LeftHookPointNormals.Empty();
	IsRight ? RightHookPointTangent.Empty() : LeftHookPointTangent.Empty();
	OnHookReleased.Broadcast(IsRight);
	IsRight ? bRightHookAttached = false : bLeftHookAttached = false;
	CheckNoLongerHooked();
	HookProjectileRelease(IsRight);
}

void AGrapplingHookCharacter::DoAirDodge(EDirection DodgeDirection)
{
	if (bIsAirDodgeCoolDown == true) return;
	bIsAirDodgeCoolDown = true;
	FVector DirectionVector = FVector();
	float Strength = 0.f;
	switch (DodgeDirection)
	{
	case EDirection::Forward:
		DirectionVector = FVector(0.f, 0.f, 1.f);
		Strength = AirDodgeUpDownMultiplier * AirDodgeStrength;
		break;
	case EDirection::Backward:
		DirectionVector = FVector(0.f, 0.f, -1.f);
		Strength = AirDodgeUpDownMultiplier * AirDodgeStrength;
		break;
	case EDirection::Right:
		DirectionVector = FollowCamera->GetRightVector();
		Strength = AirDodgeLeftRightMultiplier * AirDodgeStrength;
		break;
	case EDirection::Left:
		DirectionVector = -1.f * FollowCamera->GetRightVector();
		Strength = AirDodgeLeftRightMultiplier * AirDodgeStrength;
		break;
	}

	if (GetCharacterMovement()->IsMovingOnGround() == false ||
		UKismetMathLibrary::EqualEqual_VectorVector(UKismetMathLibrary::Normal(DirectionVector, 0.0001), FVector(0.0, 0.0, 1.0), 0.1))
	{
		GetCharacterMovement()->AddImpulse(UKismetMathLibrary::Normal(DirectionVector, 0.0001) * Strength, true);
		GetCharacterMovement()->Velocity = 
			UKismetMathLibrary::ClampVectorSize(GetCharacterMovement()->Velocity, 0.0, MaxAirBoostVelocity);
		
		AirDodgeDirection = DodgeDirection;
		switch (DodgeDirection)
		{
		case EDirection::Forward : 
			PlayAnimMontage(AirDodgeForward);
			break;
		case EDirection::Backward:
			PlayAnimMontage(AirDodgeBackward);
			break;
		case EDirection::Left:
			PlayAnimMontage(AirDodgeLeft);
			break;
		case EDirection::Right:
			PlayAnimMontage(AirDodgeRight);
			break;
		}

		if (!(bRightHookAttached || bLeftHookAttached) && bIsBoosting)
		{
			TempRotation = GetActorRotation();
			bRotationCalculationOverride = true;

			if (AirDodgeRotationCurve && AirDodgeRotationComponent)
			{
				AirDodgeRotationTrack.BindDynamic(this, &AGrapplingHookCharacter::AirDodgeRotationProgress);

				FOnTimelineEvent FinishedFunction;
				FinishedFunction.BindDynamic(this, &AGrapplingHookCharacter::AirDodgeRotationFinished);

				AirDodgeRotationComponent->AddInterpFloat(AirDodgeRotationCurve, AirDodgeRotationTrack);
				AirDodgeRotationComponent->SetTimelineFinishedFunc(FinishedFunction);
				AirDodgeRotationComponent->PlayFromStart();
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(AirDodgeTimer, this, &AGrapplingHookCharacter::AirDodgeDelay, AirDodgeCoolTime, false);

}

void AGrapplingHookCharacter::AirDodgeDelay()
{
	bIsAirDodgeCoolDown = false;
}

void AGrapplingHookCharacter::HookProjectileRelease_Implementation(bool IsRight)
{
}

void AGrapplingHookCharacter::BoostFadeOutFinished()
{
	InBoostFade = false;
}

void AGrapplingHookCharacter::EventNoLongerHooked()
{
	RotateFadeTrack.BindDynamic(this, &AGrapplingHookCharacter::RotateFadeProgress);

	if (RotateFadeCurve && RotateFadeComponent && BoostFadeOutComponent)
	{
		BoostFadeOutComponent->Stop();
		RotateFadeComponent->AddInterpFloat(RotateFadeCurve, RotateFadeTrack);
		RotateFadeComponent->PlayFromStart();
	}
}

void AGrapplingHookCharacter::EventOnReceiveHookLocation(FVector HookLocation, FVector HookNormal , bool IsRight)
{
    	if (IsRight)
	{
		if (!bRightHookAttached && bWantsRightHook)
		{
			AddHookPoint(IsRight, HookLocation, HookNormal, FVector());
		}
	}
	else
	{
		if (!bLeftHookAttached && bWantsLeftHook)
		{
			AddHookPoint(IsRight, HookLocation, HookNormal, FVector());
		}
	}
	/*
	* 	if (IsLocallyControlled())
	*/

}

void AGrapplingHookCharacter::AddHookPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent)
{
	TArray<FVector>& SelectedPointArray = IsRight? RightHookPoints : LeftHookPoints;
	SelectedPointArray.AddUnique(HookLocation);
	TArray<FVector>& SelectedNormalArray = IsRight ? RightHookPointNormals : LeftHookPointNormals;
	SelectedNormalArray.Add(HookNormal);
	TArray<FVector>& SelectedTangentArray = IsRight ? RightHookPointTangent : LeftHookPointTangent;
	SelectedTangentArray.Add(HookTangent);

	if (IsRight)
	{
		bRightHookAttached = true;
		OnBeginHooked.Broadcast();
	}
	else
	{
		bLeftHookAttached = true;
		OnBeginHooked.Broadcast();
	}
}

void AGrapplingHookCharacter::StartJumpOrBoost()
{
	if (!GetCharacterMovement()->IsMovingOnGround() || bRightHookAttached || bLeftHookAttached)
	{
		StartBoost();
	}
	else
	{
		Jump();
	}
}

void AGrapplingHookCharacter::EndJumpOrBoost()
{
	StopJumping();
	EndBoost();
	if (EndBoostFadeCurve && EndBoostFadeComponent)
	{
		EndBoostFadeTrack.BindDynamic(this, &AGrapplingHookCharacter::EndBoostFadeProgress);

		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindDynamic(this, &AGrapplingHookCharacter::EndBoostFadeFinished);

		EndBoostFadeComponent->AddInterpFloat(EndBoostFadeCurve, EndBoostFadeTrack);
		EndBoostFadeComponent->SetTimelineFinishedFunc(FinishedFunction);
		EndBoostFadeComponent->PlayFromStart();
	}
}

void AGrapplingHookCharacter::StartBoost()
{
	if (!bIsBoosting)
	{
		SetIsBoosting(true);
		OnBeginBoost.Broadcast();
		if (bRightHookAttached || bLeftHookAttached)
		{
			FVector ImpuseVector = UKismetMathLibrary::Normal(GetCurrentHookAttractionLocation() - GetActorLocation());
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hello"));
			GetCharacterMovement()->AddImpulse(ImpuseVector * BoostStartBonus, true);			
		}
	}
}

void AGrapplingHookCharacter::EndBoost()
{
	if (bIsBoosting)
	{
		SetIsBoosting(false);
		OnEndBoost.Broadcast();
	}
}

void AGrapplingHookCharacter::SetIsBoosting(bool NewIsBoosting)
{
	bIsBoosting = NewIsBoosting;
	NewIsBoosting ? GetCharacterMovement()->MaxAcceleration = 4096.0 : GetCharacterMovement()->MaxAcceleration = 2048.0;
	NewIsBoosting ? GetCharacterMovement()->GroundFriction = 0.1 : GetCharacterMovement()->GroundFriction = 2.0;
	NewIsBoosting ? GetCharacterMovement()->BrakingDecelerationWalking = 1024.0 : GetCharacterMovement()->BrakingDecelerationWalking = 1024.0;
}

void AGrapplingHookCharacter::RotateFadeProgress(float Value)
{
	FRotator LerpRotator = FMath::Lerp(FRotator(0.f, GetActorRotation().Yaw, 0.f), GetActorRotation(), Value);
	SetActorRotation(LerpRotator);
	if (!(bRightHookAttached || bLeftHookAttached))
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AGrapplingHookCharacter::CheckforHookBlock(bool IsRightHook)
{
	// IsLocallyControlled

	bIsRight = IsRightHook;
	
	bool SelectedHookAttached = false;
	if (IsRightHook)
	{
		SelectedHookAttached = bRightHookAttached;
	}
	else
	{
		SelectedHookAttached = bLeftHookAttached;
	}

	if (SelectedHookAttached)
	{
		//Check for whether the hook is blocked and update the current hook attraction point accordingly
		CheckHookAndUpdateHookPoint();

		// Check if the current blocking location for the hook is no longer being blocked. If yes, remove it from the list.
		CheckCurrentBlockLocation();

		/* Advanced hook position calculation: Will attempt to adjust the hook rope block location depending on the player position.
		In other words this will try and slide the rope along the edge to imitate actual rope behaviour. */
		CalculateHookPosition();
	}
}

void AGrapplingHookCharacter::CalculateHookPosition()
{
	TArray<FVector>& SelectedPoints = bIsRight ? RightHookPoints : LeftHookPoints;
	TArray<FVector>& SelectedNormals = bIsRight ? RightHookPointNormals : LeftHookPointNormals;
	TArray<FVector>& SelectedTangents = bIsRight ? RightHookPointTangent : LeftHookPointTangent;
	FName SocketName = bIsRight ? "LegRight" : "LegLeft";

	if (bEnableAdvancedHookLocationCalculation && SelectedPoints.Num() >= 2)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
		FVector Segment1Point = FVector();
		FVector Segment2Point = FVector();
		FVector LastNormal = SelectedNormals[SelectedNormals.Num() - 1];
		LastNormal.Normalize(0.0001);

		UKismetMathLibrary::FindNearestPointsOnLineSegments(
			SocketLocation,
			SelectedPoints[SelectedPoints.Num() - 1],
			SelectedPoints[SelectedPoints.Num() - 1] + SelectedTangents[SelectedTangents.Num() - 1] * -100.f,
			SelectedPoints[SelectedPoints.Num() - 1] + SelectedTangents[SelectedTangents.Num() - 1] * 100.f,
			Segment1Point,
			Segment2Point);

		FVector StartLocation = Segment2Point + LastNormal * HookRopeWallDistance * 15.f;
		FVector EndLocation = Segment2Point + LastNormal * HookRopeWallDistance * 15.f * -1.f;
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = false;
		CollisionParams.AddIgnoredActor(this);

		GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
			FCollisionShape::MakeSphere(HookRopeWallDistance + 5.0),
			CollisionParams
		);

		if (HitResult.bBlockingHit && !HitResult.bStartPenetrating)
		{
			FVector UpdatedLocation = HitResult.ImpactPoint + (LastNormal * HookRopeWallDistance);
			UpdateBlockPoint(bIsRight, UpdatedLocation, FVector(), FVector());
		}
	}

	// TODO :Debug
}

void AGrapplingHookCharacter::CheckCurrentBlockLocation()
{
	FName SocketName = bIsRight ? "LegRight" : "LegLeft";
	TArray<FVector>& SelectedPoint = bIsRight ? RightHookPoints : LeftHookPoints;
	if (SelectedPoint.Num() >= 2)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
		int32 LastIndex = SelectedPoint.Num() - 1;
		FVector EndLocation = FMath::Lerp(SocketLocation, SelectedPoint[LastIndex - 1], 0.99);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			SocketLocation,
			EndLocation,
			ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
			CollisionParams);

		if (!bHit)
		{
			HookBlockPointFreed(bIsRight);
		}
	}
}

void AGrapplingHookCharacter::CheckHookAndUpdateHookPoint()
{
	FName SocketName = bIsRight ? "LegRight" : "LegLeft";
	FVector StartLocation = GetMesh()->GetSocketLocation(SocketName);
	FVector EndLocation = FMath::Lerp(StartLocation, GetHookLocationOrCurrentBlockPoint(bIsRight), 0.99);
	FHitResult StartResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		StartResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
		CollisionParams);

	if (!bHit)
	{
		bHookBlockFound = false;
	}
	else
	{
		StartLocation = GetHookLocationOrCurrentBlockPoint(bIsRight);
		EndLocation = GetMesh()->GetSocketLocation(SocketName);
		FHitResult EndReslut;
		GetWorld()->LineTraceSingleByChannel(
			EndReslut,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
			CollisionParams);

		FVector SelectedNormal = FVector();
		if (bIsRight)
		{
			if (RightHookPointNormals.Num() > 0)
				SelectedNormal = RightHookPointNormals[RightHookPointNormals.Num() - 1];
		}
		else
		{
			if (LeftHookPointNormals.Num() > 0)
				SelectedNormal = LeftHookPointNormals[LeftHookPointNormals.Num() - 1];
		}
		FVector HookRopeVector = FMath::Lerp(StartResult.ImpactNormal, SelectedNormal, 0.5) * HookRopeWallDistance;
		FVector LerpedNormal = FMath::Lerp(StartResult.ImpactNormal, EndReslut.ImpactNormal, 0.5);
		LerpedNormal.Normalize(0.0001);
		FVector EdgeTangent = StartResult.ImpactNormal.Cross(EndReslut.ImpactNormal);
		HookBlockPoint(bIsRight, StartResult.ImpactPoint + HookRopeVector, LerpedNormal, EdgeTangent);
	}
}

FVector AGrapplingHookCharacter::GetHookLocationOrCurrentBlockPoint(bool ForRightHook)
{
	FVector HookLocation = FVector();
	if (ForRightHook)
	{
		if (RightHookPoints.Num() > 0)
		{
			HookLocation = RightHookPoints[RightHookPoints.Num() - 1];
		}
	}
	else
	{
		if (LeftHookPoints.Num() > 0)
		{
			HookLocation = LeftHookPoints[LeftHookPoints.Num() - 1];
		}
	}
	return HookLocation;
}

void AGrapplingHookCharacter::HookBlockPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent)
{
	// TODO : Network 
	TArray<FVector>& SelectedPoints = IsRight ? RightHookPoints : LeftHookPoints;
	TArray<FVector>& SelectedNormals = IsRight ? RightHookPointNormals : LeftHookPointNormals;
	TArray<FVector>& SelectedTangents = IsRight ? RightHookPointTangent : LeftHookPointTangent;
	IsRight ? bRightHookAttached = true : bLeftHookAttached = true;

	SelectedPoints.AddUnique(HookLocation);
	SelectedNormals.Add(HookNormal);
	SelectedTangents.Add(HookTangent);

	OnHookBlockPointFound.Broadcast(HookLocation, HookNormal, IsRight);
}

void AGrapplingHookCharacter::HookBlockPointFreed(bool IsRight)
{
	TArray<FVector>& SelectedPoints = IsRight ? RightHookPoints : LeftHookPoints;
	TArray<FVector>& SelectedNormals = IsRight ? RightHookPointNormals : LeftHookPointNormals;
	TArray<FVector>& SelectedTangents = IsRight ? RightHookPointTangent : LeftHookPointTangent;

	if (SelectedPoints.Num() > 0)
	{
		SelectedPoints.RemoveAt(SelectedPoints.Num() - 1);
	}
	if (SelectedNormals.Num() > 0)
	{
		SelectedNormals.RemoveAt(SelectedNormals.Num() - 1);
	}
	if (SelectedTangents.Num() > 0)
	{
		SelectedTangents.RemoveAt(SelectedTangents.Num() - 1);
	}

	OnHookBlockPointFreed.Broadcast(IsRight);
}

void AGrapplingHookCharacter::UpdateBlockPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent)
{
	TArray<FVector>& SelectedPoints = IsRight ? RightHookPoints : LeftHookPoints;
	SelectedPoints.Remove(SelectedPoints.Last());
	SelectedPoints.AddUnique(HookLocation);
}

void AGrapplingHookCharacter::CaculateAndAddInAirForce()
{
	if (bIsBoosting && (bRightHookAttached || bLeftHookAttached))
	{
		// Calculate Boost Force
		LeastBoostVector = CalculateBoostForce();
		GetCharacterMovement()->AddForce(LeastBoostVector * BoostForceMultiplier);
	}
	if (!GetCharacterMovement()->IsMovingOnGround() && !bIsBoosting && (bRightHookAttached || bLeftHookAttached))
	{
		// Calculate Swing Force
		LeastSwingVector = CalculateSwingForce();
		GetCharacterMovement()->AddForce(LeastSwingVector * SwingForceMultiplier);
	}
}

FVector AGrapplingHookCharacter::CalculateBoostForce()
{
	FVector BoostVector = GetCurrentHookAttractionLocation() - GetActorLocation();
	BoostVector.Normalize(0.0001);

	double Force = UKismetMathLibrary::MapRangeClamped(
		FVector::Dist(GetCurrentHookAttractionLocation(), GetActorLocation()),
		0.0,
		4000.0,
		290000.0,
		390000.0
	);
	BoostVector = BoostVector * Force + FVector(0.0, 0.0, 60000.0);
	return BoostVector;
}

FVector AGrapplingHookCharacter::GetCurrentHookAttractionLocation()
{
	if (bRightHookAttached && bLeftHookAttached)
	{
		return FMath::Lerp(GetHookLocationOrCurrentBlockPoint(true), GetHookLocationOrCurrentBlockPoint(false), 0.5);
	}
	else
	{
		if (bRightHookAttached)
		{
			return GetHookLocationOrCurrentBlockPoint(true);
		}
		else
		{
			return GetHookLocationOrCurrentBlockPoint(false);
		}
	}
	return FVector();
}

FVector AGrapplingHookCharacter::CalculateSwingForce()
{
	FVector ClampedVelocity = UKismetMathLibrary::ClampVectorSize(GetVelocity(), 400.0, MaxAirVelocity);
	FVector ActorMinusHook = GetActorLocation() - GetCurrentHookAttractionLocation();
	double DotValue1 = UKismetMathLibrary::Dot_VectorVector(ClampedVelocity, ActorMinusHook);
	ActorMinusHook.Normalize(0.0001);
	FVector DotActorMinusHook = ActorMinusHook * DotValue1 * -2.0;

	FVector HookMinusActor = GetCurrentHookAttractionLocation() - GetActorLocation();
	FVector NormalDotActorMinusHook = UKismetMathLibrary::Normal(HookMinusActor, 0.0001);
	double DotValue2 = UKismetMathLibrary::Dot_VectorVector(HookMinusActor, NormalDotActorMinusHook);
	return UKismetMathLibrary::NearlyEqual_FloatFloat(DotValue2, 1.0, 0.1) ? DotActorMinusHook : FVector();

	//Only apply force when the character is on the lower half of the swing circle
	/*
	double SwingMultiplier = GetCurrentHookAttractionLocation().Z - GetActorLocation().Z;

	SwingForce = UKismetMathLibrary::MapRangeClamped(
		SwingMultiplier,
		-1000.0,
		0.0,
		0.75,
		1.0
	); 
	*/

}

void AGrapplingHookCharacter::AdjustCamera()
{
	double ClampedFOVVelocity = UKismetMathLibrary::MapRangeClamped(
		GetVelocity().Length(),
		GetCharacterMovement()->MaxWalkSpeed,
		2100.0,
		90.0,
		100.0
	);

	FollowCamera->FieldOfView = UKismetMathLibrary::FInterpTo(
		FollowCamera->FieldOfView,
		ClampedFOVVelocity,
		UGameplayStatics::GetWorldDeltaSeconds(this), 
		4.0);

	double ClampedArmVelocity = UKismetMathLibrary::MapRangeClamped(
		GetVelocity().Length(),
		GetCharacterMovement()->MaxWalkSpeed,
		2100.0,
		400.0,
		1000.0
	);

	CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(
		CameraBoom->TargetArmLength,
		ClampedArmVelocity,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		4.0);
}

void AGrapplingHookCharacter::SpawnGroundVisualEffects()
{
	if (GetVelocity().Length() >= GroundVFXVelocityThreshold && bIsBoosting)
	{
		GroundVFXTimer += UGameplayStatics::GetWorldDeltaSeconds(this);
		if (GroundVFXTimer >= GroundVFXSpawnInterval)
		{
			GroundVFXTimer = 0.f;

			FVector StartLocation = GetActorLocation();
			FVector EndLocation = GetActorLocation() + FVector(0.f, 0.f, GroundVFXTraceLength * -1.f);
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
				CollisionParams);


			//FVector HookMinusActor = UKismetMathLibrary::Normal(GetCurrentHookAttractionLocation() - GetActorLocation(), 0.0001);
			//FRotator HookXRot = UKismetMathLibrary::MakeRotFromX(HookMinusActor);
			FRotator ActorZ = FRotator(0.f, 0.f,GetActorRotation().Yaw);
			FRotator VelocityXY = UKismetMathLibrary::MakeRotFromX(FVector(GetVelocity().X, GetVelocity().Y, 0.f));
			
			FVector SpawnLocation = HitResult.Location;
			FRotator SpawnRotation = UKismetMathLibrary::RLerp(ActorZ, VelocityXY, 0.5, true);
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation
			(
				GetWorld(),
				GroundVFX,
				SpawnLocation,
				SpawnRotation,
				FVector(1.f),
				true,
				true,
				ENCPoolMethod::None,
				true
			);

			if (NiagaraComp)
			{
				float ClampVelocity = UKismetMathLibrary::MapRangeClamped(
					GetVelocity().Length(),
					600.f,
					MaxAirBoostVelocity,
					0.01,
					1.0
				);

				float ClampDistance = UKismetMathLibrary::MapRangeClamped(
					HitResult.Distance,
					0.f,
					GroundVFXTraceLength,
					1.0,
					0.1
					);

				float ClampScale = UKismetMathLibrary::MapRangeClamped(
					ClampVelocity * ClampDistance,
					0.f,
					1.f,
					0.75,
					1.1
				);

				NiagaraComp->SetVariableFloat(TEXT("Alpha"), ClampVelocity * ClampDistance);
				NiagaraComp->SetVariableVec2(TEXT("Scale"), FVector2D(ClampScale, ClampScale));
			}

			//if (bHit)
			//{
			//	HitResult.Distance
			//}
		}

	}
}

void AGrapplingHookCharacter::SetHookVisibility()
{
	if (UKismetSystemLibrary::IsValid(LeftHookProjectile))
	{
		CableLeft->SetAttachEndTo(LeftHookProjectile, NAME_None);
		CableLeft->SetVisibility(true);
		if (bLeftHookAttached)
		{
			CableLeft->SetVisibility(false);
		}
	}
	else
	{
		CableLeft->SetVisibility(false);
	}

	if (UKismetSystemLibrary::IsValid(RightHookProjectile))
	{
		CableRight->SetAttachEndTo(RightHookProjectile, NAME_None);
		CableRight->SetVisibility(true);
		if (bRightHookAttached)
		{
			CableRight->SetVisibility(false);
		}
	}
	else
	{
		CableRight->SetVisibility(false);
	}
}

void AGrapplingHookCharacter::CalculateVelocityDotHook()
{
	FVector Velocity2D = FVector(GetVelocity().X, GetVelocity().Y, 0.0);
	Velocity2D = UKismetMathLibrary::Normal(Velocity2D, 0.001);
	FVector RotationRightVector = UKismetMathLibrary::GetRightVector(UpdateCharacterRotationWhileHooked());
	VelocityDotHook = UKismetMathLibrary::Dot_VectorVector(Velocity2D, RotationRightVector);
}

FRotator AGrapplingHookCharacter::UpdateCharacterRotationWhileHooked()
{
	//TODO : Ȯ��
	if (bLeftHookAttached || bRightHookAttached)
	{
		//Calculate the character rotation while he is hooked to a location for a smooth arc-like swinging feel
		
		FRotator NewRotation = UKismetMathLibrary::RLerp(
			UKismetMathLibrary::MakeRotFromX(GetVelocity()),
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetCurrentHookAttractionLocation()),
			0.5,
			true
		);
		NewRotation = FRotator(0.f, NewRotation.Yaw, 0.f);
		float MaxSidewaysAngle = 0.f;
		bIsBoosting ? MaxSidewaysAngle = MaxSidewaysCharacterAngleBoosting : MaxSidewaysAngle = MaxSidewaysCharacterAngle;
		float ClampAngle = UKismetMathLibrary::MapRangeClamped(
			VelocityDotHook,
			-1.0,
			1.0,
			-1.0 * MaxSidewaysAngle,
			MaxSidewaysAngle
		);
		FRotator CombineRotation = FRotator();
		if (GetCharacterMovement()->IsMovingOnGround() == false)
		{
			CombineRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(
				GetActorForwardVector(),
				ClampAngle);
		}

		NewRotation = UKismetMathLibrary::ComposeRotators(NewRotation, CombineRotation);

		HookedDesiredRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(),
			NewRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 4.0);

		if (!bRotationCalculationOverride)
		{
			SetActorRotation(HookedDesiredRotation);
		}
	}

	return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetCurrentHookAttractionLocation());
}

void AGrapplingHookCharacter::ClampMaxVelocity()
{
	if ((bLeftHookAttached || bRightHookAttached) && bIsBoosting)
	{
		GetCharacterMovement()->Velocity = UKismetMathLibrary::ClampVectorSize(
			GetCharacterMovement()->Velocity, 
			0.0, MaxAirBoostVelocity);
	}
}

void AGrapplingHookCharacter::CalculateSmartAimTargetLocations()
{
	for (int i = 1; i <= 3; i++)
	{
		int retFlag;
		CalculateLeftHookLocation(i, retFlag);
		if (retFlag == 2) break;
	}

	for (int i = 1; i <= 3; i++)
	{
		int retFlag;
		CalculateRightHookLocation(i, retFlag);
		if (retFlag == 2) break;
	}
}

void AGrapplingHookCharacter::CalculateRightHookLocation(int i, int& retFlag)
{
	retFlag = 1;
	FRotator RightAimRot = UKismetMathLibrary::ComposeRotators(
		GetBaseAimRotation(),
		RightHookSmartRotator * i
	);
	FVector StartLocation = GetMesh()->GetSocketLocation(TEXT("LegRight"));
	FVector EndLocation = GetMesh()->GetSocketLocation(TEXT("LegRight")) + UKismetMathLibrary::GetForwardVector(RightAimRot) * 10000.0;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
		CollisionParams);
	if (bHit)
	{
		SmartAimLocationRight = HitResult.ImpactPoint;
		bSmartAimRightFound = true;
		{ retFlag = 2; return; };
	}
	else
	{
		StartLocation = GetMesh()->GetSocketLocation(TEXT("LegRight")) + UKismetMathLibrary::GetForwardVector(RightAimRot) * 2500.0;
		EndLocation = GetMesh()->GetSocketLocation(TEXT("LegRight")) +
			UKismetMathLibrary::GetForwardVector(RightAimRot) * (10000.0 - SmartAimMediumTraceRadius);
		bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,	// TODO : Channel Change!!
			FCollisionShape::MakeSphere(SmartAimMediumTraceRadius),
			CollisionParams);
		if (bHit)
		{
			SmartAimLocationRight = HitResult.ImpactPoint;
			bSmartAimRightFound = true;
			{ retFlag = 2; return; };
		}
		else
		{
			StartLocation = GetMesh()->GetSocketLocation(TEXT("LegRight")) + UKismetMathLibrary::GetForwardVector(RightAimRot) * 5500.0;
			EndLocation = GetMesh()->GetSocketLocation(TEXT("LegRight")) +
				UKismetMathLibrary::GetForwardVector(RightAimRot) * (10000.0 - SmartAimLargeTraceRadius);
			bHit = GetWorld()->SweepSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel1,	// TODO : Channel Change!!
				FCollisionShape::MakeSphere(SmartAimLargeTraceRadius),
				CollisionParams);
			if (bHit)
			{
				SmartAimLocationRight = HitResult.ImpactPoint;
				bSmartAimRightFound = true;
				{ retFlag = 2; return; };
			}
			else
			{
				SmartAimLocationRight = FVector();
				bSmartAimRightFound = false;
			}
		}
	}
}

void AGrapplingHookCharacter::CalculateLeftHookLocation(int i, int& retFlag)
{
	retFlag = 1;
	FRotator LeftAimRot = UKismetMathLibrary::ComposeRotators(
		GetBaseAimRotation(),
		LeftHookSmartRotator * i
	);
	FVector StartLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft"));
	FVector EndLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft")) + UKismetMathLibrary::GetForwardVector(LeftAimRot) * 10000.0;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel1, // TODO : Channel Change!!
		CollisionParams);
	if (bHit)
	{
		SmartAimLocationLeft = HitResult.ImpactPoint;
		bSmartAimLeftFound = true;
		{ retFlag = 2; return; };
	}
	else
	{
		StartLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft")) + UKismetMathLibrary::GetForwardVector(LeftAimRot) * 2500.0;
		EndLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft")) +
			UKismetMathLibrary::GetForwardVector(LeftAimRot) * (10000.0 - SmartAimMediumTraceRadius);
		bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,	// TODO : Channel Change!!
			FCollisionShape::MakeSphere(SmartAimMediumTraceRadius),
			CollisionParams);
		if (bHit)
		{
			SmartAimLocationLeft = HitResult.ImpactPoint;
			bSmartAimLeftFound = true;
			{ retFlag = 2; return; };
		}
		else
		{
			StartLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft")) + UKismetMathLibrary::GetForwardVector(LeftAimRot) * 5500.0;
			EndLocation = GetMesh()->GetSocketLocation(TEXT("LegLeft")) +
				UKismetMathLibrary::GetForwardVector(LeftAimRot) * (10000.0 - SmartAimLargeTraceRadius);
			bHit = GetWorld()->SweepSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel1,	// TODO : Channel Change!!
				FCollisionShape::MakeSphere(SmartAimLargeTraceRadius),
				CollisionParams);
			if (bHit)
			{
				SmartAimLocationLeft = HitResult.ImpactPoint;
				bSmartAimLeftFound = true;
				{ retFlag = 2; return; };
			}
			else
			{
				SmartAimLocationLeft = FVector();
				bSmartAimLeftFound = false;
			}
		}
	}
}






