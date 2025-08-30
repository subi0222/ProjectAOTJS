// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "HLSLTree/HLSLTreeTypes.h"

#define Interactable ECC_GameTraceChannel6

#define LOG_CALL_INFO ANSI_TO_TCHAR(__FUNCTION__)
DECLARE_LOG_CATEGORY_EXTERN(LogAOT, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogAOTAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAOTGAS, Log, All);

#define AOT_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"),LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))
#define AOT_SUB_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"),LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))