// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "TitanCharacter/Data/AOTTitanAttackData.h"
#include "AOTTitanCombatStateComponent.generated.h"


class AAOTTitanBase;
class UAOTTitanAttackData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATTACKONTITAN_API UAOTTitanCombatStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAOTTitanCombatStateComponent();

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	// 남아 있는 콤보 공격이 있는지 확인
	bool HasMoreAttack() const;

	// Key에 맞는 AttackInformation이 Map에 있는지 확인
	bool HasAttackInformation(const FGameplayTag& AttackTag);

	// 진행할 수 있는 공격 중에 Target과의 거리 내에 있는 공격들이 있는지 여부 확인.
	bool IsTargetCharacterInRightDistance(TArray<FComboAttackInformation>& ICanComboAttackInformations, TArray<FComboAttackInformation>& OCanComboAttackInformations) const;

	// 진행할 수 있는 공격 중에 Target과의 거리 내에 있는 콤보 공격들이 있는지 여부 확인.
	bool IsTargetCharacterInRightDistance(TArray<FAttackInformation>& IAttackInformations, TArray<FAttackInformation>& OInRangeAttackInformations) const;

	// 공격 시작에 필요한 태그나 있지 말아야 할 태그들을 Titan상태와 비교
	bool HasTitanAcceptedTag(TArray<FAttackInformation>&IAttackInformations, TArray<FAttackInformation>& OHasAcceptedTagAttackInformations) const;
	
	// 콤보 공격에 필요한 태그나 있지 말아야 할 태그를 Titan상태와 비교
	bool HasTitanAcceptedTag(TArray<FComboAttackInformation>& IComboAttackInformations, TArray<FComboAttackInformation>& OCanComboAttackInformations) const;

	// 현재 갖고 있는 Stamina에 공격 가능한 AttackInformation들이 있는지 확인.
	bool HasEnoughStamina(TArray<FAttackInformation>&IHasAcceptedTagAttackInformations);

	// 현재 갖고 있는 Stamina에 공격 가능한 ComboAttackInformation들이 있는지 확인.
	bool HasEnoughStamina(TArray<FComboAttackInformation>& IHasAcceptedTagAttackInformations);

	//CurrentComboExactIndex 맞춰주기
	void SetCurrentComboExactIndex(int ComboIndex, FComboAttackInformation& ComboAttackInformation);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	FORCEINLINE FAttackInformation GetCurrentAttackInformation() const { return CurrentAttackInformation; }

	UFUNCTION(BlueprintCallable)
	bool CanComboAttack();

	UFUNCTION(BlueprintCallable)
	bool CanRotate();

	UFUNCTION(BlueprintCallable)
	void JumpToNextCombo();

	bool CanAttackStart(FGameplayTag& InAttackAnimModeKey);

	void SetAttackTag(FGameplayTag AttackTag);

	float GetRequiredStamina();

	bool IsAnyAttackInRange(ACharacter* Character);

	void SetAttackEnd();

	bool CanPerformAnyAttack();

	bool HasEnoughStamina() const {return bEnoughStamina;}

	TArray<FSpawnActorInformation> GetCurrentAttackSpawnActorInformations();

private:

	//현재 진행중인 AttackInformation
	FAttackInformation CurrentAttackInformation;

	//현재 진행중인 ComboAttackInforamtion
	FComboAttackInformation CurrentComboAttackInformation;

	//현재 진행 중인 Combo 공격 Index
	int8 CurrentComboIndex;

	//Combo 공격의 MontageSection을 구별하기 위한 Index
	int8 CurrentComboExactIndex;

	//진행 예정인 Combo 공격 Section Name
	FString NextComboSectionName;

	//진행 될 수 있는 AttackInformation들
	TArray<FAttackInformation> CanAttackInformations;

	//진행할 수 있는 ComboAttackInformation들
	TArray<FComboAttackInformation> CanNextComboAttackInformations;

	UPROPERTY()
	FGameplayTag CurrentAttackTag;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;

	UPROPERTY()
	TObjectPtr<AActor> Weapon;

	//몽타주 콤보 공격 등등 여러 정보들이 들어있는 데이터들을 Map형태로 받아들임.
	TMultiMap<FGameplayTag, FAttackInformation> AttackInformationMap;

	UPROPERTY()
	uint8 bCanPerformAttack : 1 = true;

	UPROPERTY()
	uint8 bEnoughStamina : 1;
	
};