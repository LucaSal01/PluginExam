// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SetNightButtonStyle.h"

class FSetNightButtonCommands : public TCommands<FSetNightButtonCommands>
{
public:

	FSetNightButtonCommands()
		: TCommands<FSetNightButtonCommands>(TEXT("SetNightButton"), NSLOCTEXT("Contexts", "SetNightButton", "SetNightButton Plugin"), NAME_None, FSetNightButtonStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
