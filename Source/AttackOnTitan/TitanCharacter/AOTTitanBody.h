// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOTTitanBody.generated.h"

class UBoxComponent;

UCLASS()
class ATTACKONTITAN_API AAOTTitanBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOTTitanBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

};
