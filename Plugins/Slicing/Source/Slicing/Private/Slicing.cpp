// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Slicing.h"
#include "SlicingStyle.h"
#include "SlicingCommands.h"
//#include "LevelEditor.h"
#include "StaticMeshEditorModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName SlicingTabName("Slicing");

#define LOCTEXT_NAMESPACE "FSlicingModule"

void FSlicingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSlicingStyle::Initialize();
	FSlicingStyle::ReloadTextures();

	FSlicingCommands::Register();
	
	PluginCommandList = MakeShareable(new FUICommandList);
	const FSlicingCommands& Commands = FSlicingCommands::Get();

	PluginCommandList->MapAction(
		Commands.OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSlicingModule::PluginButtonClicked),
		FCanExecuteAction());

	PluginCommandList->MapAction(
		Commands.CreateHandle,
		FExecuteAction::CreateRaw(this, &FSlicingModule::CreateHandle)
	);
	PluginCommandList->MapAction(
		Commands.CreateBlade,
		FExecuteAction::CreateRaw(this, &FSlicingModule::CreateBlade)
	);
	PluginCommandList->MapAction(
		Commands.CreateCuttingExitpoint,
		FExecuteAction::CreateRaw(this, &FSlicingModule::CreateCuttingExitpoint)
	);
		
	IStaticMeshEditorModule& StaticMeshEditorModule =
		FModuleManager::Get().LoadModuleChecked<IStaticMeshEditorModule>("StaticMeshEditor");
	
	// Add menubar entry
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuBarExtension(
		"Collision",
		EExtensionHook::After,
		PluginCommandList,
		FMenuBarExtensionDelegate::CreateRaw(this, &FSlicingModule::AddSlicingMenuBar)
	);
	StaticMeshEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Add toolbar entry
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Command",
		EExtensionHook::After,
		PluginCommandList,
		FToolBarExtensionDelegate::CreateRaw(this, &FSlicingModule::AddSlicingToolbar)
	);
	StaticMeshEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);


	// Add menu entry
	/*{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSlicingModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	// Add toolbar entry
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSlicingModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}*/
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlicingTabName, FOnSpawnTab::CreateRaw(this, &FSlicingModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSlicingTabTitle", "Slicing"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSlicingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlicingStyle::Shutdown();

	FSlicingCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlicingTabName);
}

TSharedRef<SDockTab> FSlicingModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSlicingModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Slicing.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSlicingModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SlicingTabName);
}

void FSlicingModule::CreateHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED HANDLE"));
}

void FSlicingModule::CreateBlade()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED BLADE"));
}

void FSlicingModule::CreateCuttingExitpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED CUTTING EXITPOINT"));
}

static void CreateSlicingMenu(FMenuBuilder& Builder)
{
	const FSlicingCommands& Commands = FSlicingCommands::Get();
	
	Builder.BeginSection("SlicingAddSlicingElements");
	{
		Builder.AddMenuEntry(Commands.CreateHandle);
		Builder.AddMenuEntry(Commands.CreateBlade);
		Builder.AddMenuEntry(Commands.CreateCuttingExitpoint);
	}
	Builder.EndSection();
}

void FSlicingModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSlicingCommands::Get().OpenPluginWindow);
}

void FSlicingModule::AddSlicingMenuBar(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		LOCTEXT("SlicingPluginMenu", "Slicing"),
		LOCTEXT("SlicingPluginMenu_ToolTip", "Opens a menu with commands for creating the elements needed to make the static mesh be able to slice."),
		FNewMenuDelegate::CreateStatic(CreateSlicingMenu),
		"Slicing"
	);
	
	//Builder.AddMenuEntry(FSlicingCommands::Get().OpenPluginWindow);
}

void FSlicingModule::AddSlicingToolbar(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSlicingCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlicingModule, Slicing)