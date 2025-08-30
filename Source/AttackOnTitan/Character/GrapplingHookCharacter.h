// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "GrapplingHookCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEventOccured);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventOccured_One, bool, IsRight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceiveHookLocation, FVector, HookLocation, FVector, HookNormal, bool, IsRight);

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AGrapplingHookCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrapplingHookCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Delegate 
	UPROPERTY(BlueprintAssignable)
	FOnEventOccured OnBeginHooked;

	UPROPERTY(BlueprintAssignable)
	FOnEventOccured OnBeginBoost;

	UPROPERTY(BlueprintAssignable)
	FOnEventOccured OnEndBoost;

	UPROPERTY(BlueprintAssignable)
	FOnEventOccured NoLongerHooked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnReceiveHookLocation OnReceiveHookLocation;

	// HookProjectile Dellegate
	UPROPERTY(BlueprintAssignable)
	FOnReceiveHookLocation OnHookBlockPointFound;

	UPROPERTY(BlueprintAssignable)
	FOnEventOccured_One OnHookBlockPointFreed;

	UPROPERTY(BlueprintAssignable)
	FOnEventOccured_One OnHookReleased;
	
	UFUNCTION()
	void EventOnBeginHooked();

	UFUNCTION()
	void EventOnBeginBoost();

	UFUNCTION()
	void EventOnEndBoost();

	UFUNCTION()
	void EventNoLongerHooked();

	UFUNCTION()
	void EventOnReceiveHookLocation(FVector HookLocation, FVector HookNormal, bool IsRight);

	void AddHookPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent);

	UFUNCTION()
	void StartJumpOrBoost();

	UFUNCTION()
	void EndJumpOrBoost();

	UFUNCTION()
	void StartBoost();

	UFUNCTION()
	void EndBoost();

	void SetIsBoosting(bool NewIsBoosting);
protected:
	// Functions
	void CheckforHookBlock(bool IsRightHook);
	void CalculateHookPosition();
	void CheckCurrentBlockLocation();
	void CheckHookAndUpdateHookPoint();
	FVector GetHookLocationOrCurrentBlockPoint(bool ForRightHook);
	void HookBlockPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent);
	void HookBlockPointFreed(bool IsRight);
	void UpdateBlockPoint(bool IsRight, FVector HookLocation, FVector HookNormal, FVector HookTangent);

	bool bHookBlockFound = false;
	bool bEnableAdvancedHookLocationCalculation = false;

	void CaculateAndAddInAirForce();
	// Calculate Boost Force
	FVector CalculateBoostForce();
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentHookAttractionLocation();
	// Calculate Swing Force
	FVector CalculateSwingForce();

	void AdjustCamera();
	void SpawnGroundVisualEffects();
	void SetHookVisibility();
	void CalculateVelocityDotHook();
	FRotator UpdateCharacterRotationWhileHooked();

	void ClampMaxVelocity();
	void CalculateSmartAimTargetLocations();

	void CalculateRightHookLocation(int i, int& retFlag);

	void CalculateLeftHookLocation(int i, int& retFlag);

	UFUNCTION()
	void BoostFadeInProgress(float Value);

	UFUNCTION()
	void BoostFadeInFinished();

	UFUNCTION()
	void BoostFadeOutProgress(float Value);

	UFUNCTION()
	void BoostFadeOutFinished();


	UFUNCTION()
	void RotateFadeProgress(float Value);

	UFUNCTION()
	void SwingFadeInProgress(float Value);

	UFUNCTION()
	void EndBoostFadeProgress(float Value);

	UFUNCTION()
	void EndBoostFadeFinished();

	UFUNCTION()
	void AirDodgeRotationProgress(float Value);

	UFUNCTION()
	void AirDodgeRotationFinished();

	UFUNCTION()
	void CheckNoLongerHooked();
public:
	UFUNCTION()
	void FireHookProjectile(bool IsRight);

	UFUNCTION(BlueprintNativeEvent)
	void HookProjectileFire(bool IsRight, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION()
	void ReleaseHookProjectile(bool IsRight);

	UFUNCTION(BlueprintNativeEvent)
	void HookProjectileRelease(bool IsRight);

	UFUNCTION(BlueprintCallable)
	void DoAirDodge(EDirection DodgetDirection);

	void AirDodgeDelay();

	// Variables
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* BoostFadeInComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* BoostFadeOutComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* SwingFadeInComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* RotateFadeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* EndBoostFadeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeLine")
	UTimelineComponent* AirDodgeRotationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* BoostFadeInCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* EndBoostFadeCurve;

	FOnTimelineFloat EndBoostFadeTrack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* BoostFadeOutCurve;

	FOnTimelineFloat SwingFadeInTrack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* SwingFadeInCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* RotateFadeCurve;

	FOnTimelineFloat BoostFadeInTrack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* AirDodgeRotationCurve;

	FOnTimelineFloat AirDodgeRotationTrack;

	// Boost
	FVector LeastBoostVector = FVector();
	FVector LeastSwingVector = FVector();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Boost")
	bool bIsBoosting = false;
	bool InBoostFade = false;
	float BoostForceMultiplier = 1.f;
	float SwingForceMultiplier = 1.f;
	float BoostStartBonus = 750.0;
	float GroundFriction = 2.0;
	float BrakingDecelerationWalking = 1024.0;
	float MaxAcceleration = 2048.0;

	FOnTimelineFloat BoostFadeOutTrack;

	FOnTimelineFloat RotateFadeTrack;

	//Hook
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bIsRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bIsRightLocal = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bLeftHookAttached = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bRightHookAttached = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bWantsRightHook = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	bool bWantsLeftHook = false;

	float MaxSidewaysCharacterAngle = 35.f;
	float MaxSidewaysCharacterAngleBoosting = 75.f;
	float VelocityDotHook = 0.f;
	FRotator HookedDesiredRotation = FRotator();
	bool bRotationCalculationOverride = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hook")
	double HookRopeWallDistance = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hook")
	TArray<FVector> RightHookPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hook")
	TArray<FVector> LeftHookPoints;
	TArray<FVector> RightHookPointNormals;
	TArray<FVector> LeftHookPointNormals;
	TArray<FVector> RightHookPointTangent;
	TArray<FVector> LeftHookPointTangent;

	//Movement
	float MaxAirVelocity = 2048.0;
	float MaxAirBoostVelocity = 3250.0;

	// VFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	class UNiagaraSystem* GroundVFX;

	float GroundVFXVelocityThreshold = 750.f;
	float GroundVFXTimer = 0.f;
	float GroundVFXSpawnInterval = 0.035f;
	float GroundVFXTraceLength = 300.f;

	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hook")
	TSubclassOf<class AActor> HookProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook")
	class AActor* RightHookProjectile; // TODO : HookProjectile 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook")
	class AActor* LeftHookProjectile;  // TODO : HookProjectile 

	// Cable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cable")
	class UCableComponent* CableLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cable")
	class UCableComponent* CableRight;

	// Smart Aim
	FRotator RightHookSmartRotator = FRotator(0.f, 0.f, 11.f);
	FRotator LeftHookSmartRotator = FRotator(0.f, 0.f, -11.f);

	FVector SmartAimLocationRight = FVector();
	FVector SmartAimLocationLeft = FVector();

	bool bSmartAimRightFound = false;
	bool bSmartAimLeftFound = false;
	
	float SmartAimMediumTraceRadius = 250.0;
	float SmartAimLargeTraceRadius = 700.0;

	bool bIsSmartAiming = false;

	// AirDodge
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	class UAnimMontage* AirDodgeForward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	class UAnimMontage* AirDodgeBackward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	class UAnimMontage* AirDodgeLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	class UAnimMontage* AirDodgeRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	float AirDodgeUpDownMultiplier = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	float AirDodgeLeftRightMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	float AirDodgeStrength = 2000.f;

	EDirection AirDodgeDirection = EDirection::Forward;
	FRotator TempRotation = FRotator();

	FTimerHandle AirDodgeTimer;
	bool bIsAirDodgeCoolDown = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirDodge")
	float AirDodgeCoolTime = 1.f;
};
