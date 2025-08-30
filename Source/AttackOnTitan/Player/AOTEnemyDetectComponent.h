// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AOTEnemyDetectComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangeActor, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UAOTEnemyDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAOTEnemyDetectComponent();

	AActor* DetectEnemy(float Range, float Radius);
	AActor* NextEnemy();

	void ResetDetectEnemy();

	FORCEINLINE AActor* GetCurrentActor() { return CurrentActor; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void UpdateCurrentActor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FOnChangeActor OnChangeActor;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> DetectEnemies;
	UPROPERTY()
	TObjectPtr<AActor> CurrentActor;
	int32 CurrentIndex = -1;
	bool bDetecting = false;
	float TargetDetectRadius = 0.f;
	float CurrentDetectRadius = 0.f;

};
