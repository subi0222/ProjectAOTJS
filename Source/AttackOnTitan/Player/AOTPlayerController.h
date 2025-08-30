// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AOTPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFinishMoveToActor);

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void HandleMouseTrace();
	void OnMouseClick();
	void MoveCharacterToTarget(float DeltaTime);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void FireRightHookProjectile();
	void FireLeftHookProjectile();
	void ReleaseRightHookProjectile();
	void ReleaseLeftHookProjectile();
	void StartJumpAndBoost();
	void EndJumpAndBoost();

	void DoAirDodgeForward();
	void DoAirDodgeBackward();
	void DoAirDodgeLeft();
	void DoAirDodgeRight();
public:
	FOnFinishMoveToActor OnFinishMoveToActor;

protected:
	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* SRContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	FVector2D LookSensitivity = FVector2D(1.0f, 1.0f);

	// Input actions for character behavior
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightHookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftHookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AirDodgeForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AirDodgeBackwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AirDodgeLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AirDodgeRightAction;
private:
	FVector TargetLocation;
	bool bShouldMove = false;

	// AirDodge


	class AGrapplingHookCharacter* Character;
};
