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

	void CreateHandle();
	void CreateBlade();
	void CreateCuttingExitpoint();
	void ShowSlicingElements();

	UObject * Mesh;
	IAssetEditorInstance * Editore;
	
private:
	TSharedPtr<class FViewport> StaticMeshEditorViewport;
	TSharedPtr<class FUICommandList> PluginCommandList;

	// Functions to differentiate the different initialization stages
	void InitializeUIButtons();
	void AddUIButtons();

	// Refreshes the viewport to show newly created objects
	void RefreshViewport();
	
	// Handling Assets
	void HandleAsset(UObject * Asset, IAssetEditorInstance *Editor);

	void AddSlicingToolbar(FToolBarBuilder& Builder);
	void AddSlicingMenuBar(FMenuBarBuilder& Builder);
};