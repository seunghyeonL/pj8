#pragma once

#include "CoreMinimal.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EWaveType : uint8
{
	Wave_1, Wave_2, Wave_3, Wave_End
};

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	Wave_Waiting, Wave_Running, Wave_End 
};