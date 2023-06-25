// Copyright Epic Games, Inc. All Rights Reserved.

#include "SetNightButtonCommands.h"

#define LOCTEXT_NAMESPACE "FSetNightButtonModule"

void FSetNightButtonCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SetNightButton", "Execute SetNightButton action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
