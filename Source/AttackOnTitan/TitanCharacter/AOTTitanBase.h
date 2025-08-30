// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interface/AOTTitanAttackInterFace.h"
#include "GameFramework/Character.h"
#include "AOTTitanBase.generated.h"

class AAOTTitanPath;
class UAOTTitanBodyTargetWidgetComponent;
class UAOTTitanHealthComponent;
class UAOTTitanCombatAttributeSet;
class UAOTTitanHealthAttributeSet;
class UAOTTitanCombatStateComponent;
class UAOTTitanHealthBar;
class UAOTTitanHPWidgetComponent;
class UWidgetComponent;
class UAOTTitanAttackData;
class UAOTTitanStatData;
class UAOTTitanTargetControlComponent;
class UGameplayAbility;
class UAIPerceptionComponent;
class UAOTGA_SeveredBodyPart;
class UAOTGA_TitanAttack;
class UAOTTitanGameplayAbility;
class UGameplayEffect;
class UMotionWarpingComponent;
class AAOTTitanControllerBase;
class UAOTTitanAbilitySystemComponent;

//공격이 끝났을 때, 호출
DECLARE_DELEGATE(FOnAttackEndDelegate)

//공격을 할 수 없을 때, BTTask에 호출
DECLARE_DELEGATE(FOnCannotAttackDelegate)

UCLASS()
class ATTACKONTITAN_API AAOTTitanBase : public ACharacter, public IAbilitySystemInterface,
                                        public IAOTTitanAttackInterFace
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAOTTitanBase();

	//Data Asset접근
	FORCEINLINE UAOTTitanStatData* GetTitanStatData() const { return TitanStatData; }

	FORCEINLINE UAOTTitanAttackData* GetTitanAttackData() const { return TitanAttackData; }

	FORCEINLINE void SetAttackEndDelegate(const FOnAttackEndDelegate& InOnAttackEndDelegate)
	{
		OnAttackEndDelegate = InOnAttackEndDelegate;
	}

	FORCEINLINE UAOTTitanBodyTargetWidgetComponent* GetWeaknessWidgetComponent() const
	{
		return WeaknessTargetWidgetComp;
	}

	FORCEINLINE UAOTTitanBodyTargetWidgetComponent* GetBodyTargetWidgetComponent() const
	{
		return BodyTargetWidgetComp;
	}

	FORCEINLINE bool HasPath() const { return TitanPath != nullptr; }

	FORCEINLINE AAOTTitanPath* GetTitanPath() const { return TitanPath; }

	FORCEINLINE void AddSpawnActor(AActor* SpawnActor) {SpawnActors.Add(SpawnActor);}

	FORCEINLINE TArray<AActor*> GetSpawnActors() const { return SpawnActors; }

	FORCEINLINE void RemoveSpawnActor(AActor* SpawnActor) {SpawnActors.Remove(SpawnActor);}

	virtual void TryAttack(FGameplayTag AttackTag) override;

	//IAbilitySystemInterface 구현부
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//여기서 바인딩하는게 좋을 것 같다.
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	// Called whaen the game starts or when spawned
	virtual void BeginPlay() override;

	//GA에서 접근
	void NotifyAttackEnd() const;

protected:
	UFUNCTION()
	void SetHealthPercent(float Percent);

	void ApplyDataToHealthAttributeSet();

	void ApplyDataToCombatAttributeSet();


	//감지 Comp
	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	//동작 Comp
	UPROPERTY(EditDefaultsOnly, Category = MotionWarp)
	TObjectPtr<UMotionWarpingComponent> MotionWarpComp;

	//타겟 Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TargetControl)
	TObjectPtr<UAOTTitanTargetControlComponent> TargetControlComp;

	//HPWidget Comp
	UPROPERTY(EditDefaultsOnly, Category = HPWidget)
	TObjectPtr<UWidgetComponent> HPWidgetComp;

	//CombatState Comp
	UPROPERTY(EditDefaultsOnly, Category = CombatState)
	TObjectPtr<UAOTTitanCombatStateComponent> CombatStateComp;

	//HealthComp
	UPROPERTY(EditDefaultsOnly, Category = Health)
	TObjectPtr<UAOTTitanHealthComponent> HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = TargetWidget)
	TObjectPtr<UAOTTitanBodyTargetWidgetComponent> BodyTargetWidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = TargetWidget)
	TObjectPtr<UAOTTitanBodyTargetWidgetComponent> WeaknessTargetWidgetComp;


	//Titan과 관련된 Data 모음
	//Stat과 관련된 Data
	/*
	 * Target
	 * 기본 HP
	 * Detect Radius
	 */
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TObjectPtr<UAOTTitanStatData> TitanStatData;

	//공격 관련된 Data
	/*
	 * 
	 */
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TObjectPtr<UAOTTitanAttackData> TitanAttackData;

	//UUserWidget 클래스 결정
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TSubclassOf<UAOTTitanHealthBar> TitanHpWidgetClass;

	//Gameplay Ability와 관련있는 것들
	//ASC
	UPROPERTY()
	TObjectPtr<UAOTTitanAbilitySystemComponent> TitanASC;

	//시작 스탯을 넣기 위한 Effect
	UPROPERTY(EditDefaultsOnly, Category = GamePlayAbility)
	TSubclassOf<UGameplayEffect> InitEffectClass;

	//근접 주먹 공격 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = GamePlayAbility)
	TSubclassOf<UAOTGA_TitanAttack> MeleeAttackAbilityClass;

	//스킬 공격 어빌리티
	// UPROPERTY(EditDefaultsOnly, Category = GamePlayAbility)
	// TSubclassOf<UAOTGA_TitanSkillAttack> SpecialAttackAbility;

	//신체가 잘리는 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = GamePlayAbility)
	TSubclassOf<UAOTGA_SeveredBodyPart> SeveredAbilityClass;

	//그 외 나머지 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = GameplayAbility)
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilityClasses;

	UPROPERTY(EditAnywhere, Category = Path)
	TObjectPtr<AAOTTitanPath> TitanPath;

	UPROPERTY()
	TObjectPtr<UAOTTitanHealthAttributeSet> HealthAttributeSet;

	UPROPERTY()
	TObjectPtr<UAOTTitanCombatAttributeSet> CombatAttributeSet;

	//AIController
	UPROPERTY()
	TObjectPtr<AAOTTitanControllerBase> TitanControllerBase;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnActors;

private:
	FOnAttackEndDelegate OnAttackEndDelegate;
};
