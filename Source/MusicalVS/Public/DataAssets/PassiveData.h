
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PassiveData.generated.h"


UCLASS()
class MUSICALVS_API UPassiveData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PassiveValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> LevelUps;

};
