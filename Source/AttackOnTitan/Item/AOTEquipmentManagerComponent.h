// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AOTEquipmentManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UAOTEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAOTEquipmentManagerComponent();
	void EquipItem();

protected:
	virtual void BeginPlay() override;

public:
		
};
