// Copyright 2017, Institute for Artificial Intelligence

#include "SlicingEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSlicingEditorModule"

void FSlicingEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow,
		"Slicing", "Bring up Slicing window", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ShowSlicingElements, "Slicing",
		"Toggles display of the slicing elements in the Preview Pane.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(CreateHandle, "Add Handle",
		"Generates a new box to signify the area used as the handle.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateBlade, "Add Blade",
		"Generates a new box to signify the area used as the blade.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateCuttingExitpoint, "Add Cutting Exitpoint",
		"Generates a new box to signify the area at which cutting is supposed to be aborted.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE