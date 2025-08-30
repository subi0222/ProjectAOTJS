// Unreal Engine Marketplace Copyright


#include "AOTPlayerController.h"
#include "Components/BoxComponent.h"
#include "AttackOnTitan/Character/AOTBasePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Character/GrapplingHookCharacter.h"

void AAOTPlayerController::Tick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    //HandleMouseTrace();

    //if (bShouldMove)
    //    MoveCharacterToTarget(DeltaTime);
}

void AAOTPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = false;

    //FInputModeGameAndUI InputMode;
    //InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    //InputMode.SetHideCursorDuringCapture(true);
    //InputMode.SetWidgetToFocus(nullptr);

    //SetInputMode(InputMode);

    Character = Cast<AGrapplingHookCharacter>(GetCharacter());

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(SRContext, 0);
    }
}

void AAOTPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::Look);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::Move);
    EnhancedInputComponent->BindAction(RightHookAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::FireRightHookProjectile);
    EnhancedInputComponent->BindAction(LeftHookAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::FireLeftHookProjectile);

    EnhancedInputComponent->BindAction(RightHookAction, ETriggerEvent::Completed, this, &AAOTPlayerController::ReleaseRightHookProjectile);
    EnhancedInputComponent->BindAction(LeftHookAction, ETriggerEvent::Completed, this, &AAOTPlayerController::ReleaseLeftHookProjectile);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::StartJumpAndBoost);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAOTPlayerController::EndJumpAndBoost);

    EnhancedInputComponent->BindAction(AirDodgeForwardAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::DoAirDodgeForward);
    EnhancedInputComponent->BindAction(AirDodgeBackwardAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::DoAirDodgeBackward);
    EnhancedInputComponent->BindAction(AirDodgeLeftAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::DoAirDodgeLeft);
    EnhancedInputComponent->BindAction(AirDodgeRightAction, ETriggerEvent::Triggered, this, &AAOTPlayerController::DoAirDodgeRight);

}

void AAOTPlayerController::HandleMouseTrace()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        UBoxComponent* BoxComponent = Cast<UBoxComponent>(HitResult.GetComponent());
        if (BoxComponent)
        {
            FVector BoxLocation = BoxComponent->GetComponentLocation();
            FVector BoxExtent = BoxComponent->GetScaledBoxExtent();


            DrawDebugBox(GetWorld(), BoxLocation, BoxExtent, FQuat::Identity, FColor::Red, false, -1.0f, 0, 2.0f);
        }
    }
}

void AAOTPlayerController::OnMouseClick()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);

    UBoxComponent* BoxComponent = Cast<UBoxComponent>(HitResult.GetComponent());
    if (BoxComponent)
    {
        TargetLocation = HitResult.Location;
        bShouldMove = true;

        SetIgnoreMoveInput(true);
    }
}

void AAOTPlayerController::MoveCharacterToTarget(float DeltaTime)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    float Speed = 600.0f;
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;


    if (FVector::Dist(CurrentLocation, TargetLocation) < 200.0f)
    {
        bShouldMove = false;
        SetIgnoreMoveInput(false);
        OnFinishMoveToActor.Broadcast();
    }
    else
    {
        ControlledPawn->SetActorLocation(NewLocation);
    }
}

void AAOTPlayerController::Look(const FInputActionValue& Value)
{    // 입력 축 (Vector2D) * 감도
    const FVector2D LookAxisVector = Value.Get<FVector2D>() * LookSensitivity;

    // APlayerController에는 AddYawInput / AddPitchInput 사용 가능
    GetPawn()->AddControllerYawInput(LookAxisVector.X);
    GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
}

void AAOTPlayerController::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    // find out which way is forward
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    // get right vector 
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // add movement 
    GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
    GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void AAOTPlayerController::FireRightHookProjectile()
{
    Character->FireHookProjectile(true);
}

void AAOTPlayerController::FireLeftHookProjectile()
{
    Character->FireHookProjectile(false);
}

void AAOTPlayerController::ReleaseRightHookProjectile()
{
    Character->ReleaseHookProjectile(true);
}

void AAOTPlayerController::ReleaseLeftHookProjectile()
{
    Character->ReleaseHookProjectile(false);
}

void AAOTPlayerController::StartJumpAndBoost()
{
    Character->StartJumpOrBoost();
}

void AAOTPlayerController::EndJumpAndBoost()
{
    Character->EndJumpOrBoost();
}

void AAOTPlayerController::DoAirDodgeForward()
{
    Character->DoAirDodge(EDirection::Forward);
}

void AAOTPlayerController::DoAirDodgeBackward()
{
    Character->DoAirDodge(EDirection::Backward);
}

void AAOTPlayerController::DoAirDodgeLeft()
{
    Character->DoAirDodge(EDirection::Left);
}

void AAOTPlayerController::DoAirDodgeRight()
{
    Character->DoAirDodge(EDirection::Right);
}
