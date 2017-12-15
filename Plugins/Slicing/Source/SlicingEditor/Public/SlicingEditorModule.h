// Copyright 2017, Institute for Artificial Intelligence

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

//#include "FSceneViewport.h"

class FToolBarBuilder;
class FMenuBuilder;
class FViewport;

class FSlicingEditorModule: public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Debug option booleans to be read  */
	bool bEnableDebugConsoleOutput = true;
	bool bEnableDebugShowPlane = false;
	bool bEnableDebugShowTrajectory = false;

	/** Slicing editor actions */
	// Static mesh editor
	void CreateHandle();
	void CreateBlade();
	void CreateCuttingExitpoint();
	void ShowSlicingElements();
	// Level editor debug options
	void OnEnableDebugConsoleOutput();
	bool OnIsEnableDebugConsoleOutputEnabled();
	void OnEnableDebugShowPlane();
	bool OnIsEnableDebugShowPlaneEnabled();
	void OnEnableDebugShowTrajectory();
	bool OnIsEnableDebugShowTrajectoryEnabled();

	UObject * Mesh;
	IAssetEditorInstance * Editore;
	
private:
	TSharedPtr<class FViewport> StaticMeshEditorViewport;
	TSharedPtr<class FUICommandList> PluginCommandList;

	/** Creating the UI elements */
	void InitializeUIButtons();
	void AddUIButtons();
	void CreateDebugButtons();

	// Refreshes the viewport to show newly created objects
	void RefreshViewport();
	
	// Handling Assets
	void HandleAsset(UObject * Asset, IAssetEditorInstance *Editor);

	/** Needed to build the UI elements */
	void CreateSlicingMenu(FMenuBuilder& Builder);
	TSharedRef<SWidget> CreateDebugOptionMenu();
	void AddSlicingMenuBar(FMenuBarBuilder& Builder);
	void AddSlicingToolbar(FToolBarBuilder& Builder);
	void AddDebugOptions(FToolBarBuilder& Builder);
};