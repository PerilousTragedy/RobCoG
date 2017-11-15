// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SlicingCommands.h"

#define LOCTEXT_NAMESPACE "FSlicingModule"

void FSlicingCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Slicing", "Bring up Slicing window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
