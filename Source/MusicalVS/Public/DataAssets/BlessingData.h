
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BlessingData.generated.h"

class ABlessingSystem;
class UFMODEvent;

UCLASS()
class MUSICALVS_API UBlessingData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> BlessingMarkers;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFMODEvent> SoundEvent;
	
};
