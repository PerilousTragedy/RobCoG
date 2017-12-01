// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlicingStyle.h"

class FSlicingCommands : public TCommands<FSlicingCommands>
{
public:
	FSlicingCommands(): TCommands<FSlicingCommands>(
		TEXT("Slicing"), // Context name for fast lookup
		NSLOCTEXT("Contexts", "Slicing", "Slicing Plugin"), // Localized context name
		NAME_None, // Parent
		FSlicingStyle::GetStyleSetName() // Icon Style Set
		){}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> OpenPluginWindow;
	TSharedPtr<FUICommandInfo> ShowSlicingElements;
	TSharedPtr<FUICommandInfo> CreateHandle;
	TSharedPtr<FUICommandInfo> CreateBlade;
	TSharedPtr<FUICommandInfo> CreateCuttingExitpoint;
};