// Copyright 2017, Institute for Artificial Intelligence

#include "SlicingEditorActionCallbacks.h"

#include "IStaticMeshEditor.h"
#include "StaticMeshEditorActions.h"
#include "StaticMeshEditorModule.h"

#define LOCTEXT_NAMESPACE "FSlicingEditorModule"

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
	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugConsoleOutputEnabled(bool* bButtonValue)
{
	return *bButtonValue;
}

void FSlicingEditorActionCallbacks::OnEnableDebugShowPlane(bool* bButtonValue)
{
	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugShowPlaneEnabled(bool* bButtonValue)
{
	return *bButtonValue;
}

void FSlicingEditorActionCallbacks::OnEnableDebugShowTrajectory(bool* bButtonValue)
{
	*bButtonValue = !*bButtonValue;
}

bool FSlicingEditorActionCallbacks::OnIsEnableDebugShowTrajectoryEnabled(bool* bButtonValue)
{
	return *bButtonValue;
}

void FSlicingEditorActionCallbacks::ReplaceSocketsWithUseableComponents()
{

}

#undef LOCTEXT_NAMESPACE