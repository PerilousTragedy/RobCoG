// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlicingStyle.h"

class FSlicingCommands : public TCommands<FSlicingCommands>
{
public:

	FSlicingCommands()
		: TCommands<FSlicingCommands>(TEXT("Slicing"), NSLOCTEXT("Contexts", "Slicing", "Slicing Plugin"), NAME_None, FSlicingStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};