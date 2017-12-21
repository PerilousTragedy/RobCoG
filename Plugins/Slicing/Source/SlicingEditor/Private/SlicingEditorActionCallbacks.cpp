#include "SlicingEditorActionCallbacks.h"

#include "IStaticMeshEditor.h"
#include "StaticMeshEditorActions.h"
#include "StaticMeshEditorModule.h"

#include "Engine.h"

void FSlicingEditorActionCallbacks::CreateBlade()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED BLADE"));
}

void FSlicingEditorActionCallbacks::CreateCuttingExitpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED CUTTING EXITPOINT"));
}

void FSlicingEditorActionCallbacks::ShowSlicingElements()
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED SLICING ELEMENTS SHOWN"));
}

void FSlicingEditorActionCallbacks::OnEnableDebugConsoleOutput(bool* bButtonValue)
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG CONSOLE OUTPUT"));

	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugConsoleOutputEnabled(bool* bButtonValue)
{
	return *bButtonValue == true;
}

void FSlicingEditorActionCallbacks::OnEnableDebugShowPlane(bool* bButtonValue)
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG SHOWING SLICING PLANE"));

	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugShowPlaneEnabled(bool* bButtonValue)
{
	return *bButtonValue == true;
}

void FSlicingEditorActionCallbacks::OnEnableDebugShowTrajectory(bool* bButtonValue)
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG SHOWING SLICING TRAJECTORY"));

	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugShowTrajectoryEnabled(bool* bButtonValue)
{
	return *bButtonValue == true;
}