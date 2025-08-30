// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackOnTitan/Interface/AOTAnimInterface_Player.h"
#include "InputActionValue.h"
#include "AOTGameplayTag.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AHookProjectileBase;
class UAOTBaseComboAttackData;
class AAOTWeapon_Spear;
class UAOTEnemyDetectComponent;
class UMotionWarpingComponent;
class UAOTSkillData;
class UInteractionComponent;
class UInventoryComponent;
class UAOTPlayerAbilitySystemComponent;
class UGameplayAbility;

UENUM(BlueprintType)
enum class Mode : uint8
{
	DualBlade,
	Spear,
	LockIn,
	Titan,
	End,
};


UCLASS()
class ATTACKONTITAN_API ABaseCharacter : public ACharacter, public IAOTAnimInterface_Player , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	FORCEINLINE UAOTBaseComboAttackData* GetBaseComboAttackData() { return AttackBaseComboData; } // BaseAttackGA���� ���
	FORCEINLINE UAOTBaseComboAttackData* GetBaseJumpAttackComboData() { return JumpAttackBaseComboData; } // BaseAttackGA���� ���
	FORCEINLINE UAnimMontage* GetBaseAttackMontage(){return BaseAttackMontage;}
	
	FORCEINLINE UAnimMontage* GetRollMontage() { return RollingMontage; } // RollingGA���� ���
	FORCEINLINE UAnimMontage* GetSpearAttackMontage() { return SpearAttackMontage; } // SpearAttackGA���� ���
	FORCEINLINE UAnimMontage* GetModeChangeMontage() { return ModeChangeMontage; } // ModeChangeGA���� ���

	FORCEINLINE TSubclassOf<AAOTWeapon_Spear> GetSpearClass() { return SpearClass; } // SpawnSpearGA���� ��������� GA�� ��ü����Ǿ��־ ���Ⲩ�� �����پ�������
	FORCEINLINE USkeletalMeshComponent* GetRightWeaponSkeletalMesh() { return RightWeaponMeshComp; } // ModeChangeGA���� ���
	FORCEINLINE USkeletalMeshComponent* GetLeftWeaponSkeletalMesh() { return LeftWeaponMeshComp; } // ModeChangeGA���� ���
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() { return MotionWarpingComponent; } // ���µ� ����
	FORCEINLINE AAOTWeapon_Spear* GetRightSpear() { return RightSpear; } // ModeChangeGA���� ���
	FORCEINLINE AAOTWeapon_Spear* GetLeftSpear() { return LeftSpear; } // ModeChangeGA���� ���
	FORCEINLINE UAOTEnemyDetectComponent* GetEnemyDetectComponent() { return DetectEnemyComponent; } // SpawnSpearGA���� ���
	FORCEINLINE UAOTSkillData* GetCurrentSkillData() { return CurrentSkillData; }
	FORCEINLINE void SetCurrentSkillData(UAOTSkillData* SkillData) { CurrentSkillData = SkillData; }
	FORCEINLINE UAOTSkillData* GetSkillData(int32 Index) { return SkillDatas[Index]; }
	
	
	AHUD* GetHUD();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// *****InputFunction*****
	void OnMoveToATKPoint();
	void OnDetectEnemy();
	void OnNextDetectEnemy();
	void OnTitanMode();

	// ******** AnimInterface Function****************
	virtual void RotateToTarget() override;
	virtual void ChangeMode() override;
	
	void InputPressed(int32 InputId);
	void InputReleased(int32 InputId);

	UFUNCTION()
	void UpdateTargetActorCallback(AActor* NewActor);

	// ******** Interaction Function****************
	void Interact();

public:
	UFUNCTION(BlueprintCallable)
	void Pickup(class ABaseInteractionItem* newItem);

	UFUNCTION(BlueprintCallable)
	void UseItem(int32 index);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsDead();

public:
	//************************ Input ******************************************
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> IC_DualBladeMode;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> IC_SpearMode;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> IC_LockInMode;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> IC_TitanMode;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_ChangeMode;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_DetectEnemy;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_NextDetectEnemy;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_FireHookRight;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_FireHookLeft;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_MoveToAttackPoint;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_BaseAttack;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_SpearAttack;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Dash;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Skill01;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Skill02;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Skill03;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Skill04;


	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Rolling;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_Interact;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_TitanMode;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> ModeChangeMontage;

	//*********************************** Weapon *********************************************
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> LeftWeaponMeshComp;
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> RightWeaponMeshComp;
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMesh> LeftWeaponMesh;
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMesh> RightWeaponMesh;

	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> RightSpear;
	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> LeftSpear;
	

	Mode ModeType = Mode::DualBlade;


protected:
	//************************* ComboData *******************************
	UPROPERTY(EditAnywhere, Category = ComboData)
	TObjectPtr<UAOTBaseComboAttackData> AttackBaseComboData;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TObjectPtr<UAOTBaseComboAttackData> JumpAttackBaseComboData;	

	UPROPERTY()
	TObjectPtr<UAOTSkillData> CurrentSkillData;
	UPROPERTY(EditAnywhere, Category = ComboData)
	TObjectPtr<UAnimMontage> BaseAttackMontage;

	// ****************** Combo ****************************** 
	UPROPERTY()
	TObjectPtr<UAOTBaseComboAttackData> CurrentComboData;

	// ********************* Spear Attack ********************
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> SpearAttackMontage;

	UPROPERTY(EditAnywhere, Category = Spear)
	TSubclassOf<AAOTWeapon_Spear> SpearClass;

	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> SpawnedSpear;

	//******************** Stat ****************************

	FOnMontageEnded EndDelegate;

	//******************** Rolling *************************
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> RollingMontage;

	UPROPERTY()
	bool bRolling = false;

	//********************* Dead *****************************
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	TObjectPtr<UAnimMontage> DeadMontage;

	// ******************* Game Ability ***********************
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	// ********************* Warp Motion Component*******************
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	// ********************* 
	// Component*******************
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;
private:
	UPROPERTY()
	TObjectPtr<UAOTPlayerAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UAOTEnemyDetectComponent> DetectEnemyComponent;
	UPROPERTY()
	TObjectPtr<AActor> CurrentLockInActor;
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<TObjectPtr<UAOTSkillData>> SkillDatas;
	
};
