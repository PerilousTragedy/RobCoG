// Copyright 2017, Institute for Artificial Intelligence

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "SEditorViewport.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSlicingEditorModule: public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void CreateHandle();
	void CreateBlade();
	void CreateCuttingExitpoint();
	
private:
	TSharedPtr<class SEditorViewport> StaticMeshEditorViewport;
	TSharedPtr<class FUICommandList> PluginCommandList;

	// Refreshes the viewport to show newly created objects
	void RefreshViewport();

	void AddSlicingToolbar(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	void AddSlicingMenuBar(FMenuBarBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
};