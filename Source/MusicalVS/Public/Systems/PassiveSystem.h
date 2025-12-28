
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PassiveSystem.generated.h"

class UPassiveData;

UCLASS()
class MUSICALVS_API APassiveSystem : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UPassiveData> PassiveData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Level = 0;

protected:
	virtual void BeginPlay() override;

public:
	APassiveSystem();
	virtual void Tick(float DeltaTime) override;
};
