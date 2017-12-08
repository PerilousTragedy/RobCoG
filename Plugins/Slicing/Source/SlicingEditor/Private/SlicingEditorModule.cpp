// Copyright 2017, Institute for Artificial Intelligence

#include "SlicingEditorModule.h"
#include "SlicingEditorStyle.h"
#include "SlicingEditorCommands.h"

#include "LevelEditor.h"
#include "Editor.h"
#include "StaticMeshEditorModule.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "FSlicingEditorModule"

// This code will execute after your module is loaded into memory; the exact timing is specified in the
// .uplugin file per-module
void FSlicingEditorModule::StartupModule()
{
	/** Initialize the appearance of the UI buttons */
	FSlicingEditorStyle::Initialize();
	FSlicingEditorStyle::ReloadTextures();

	InitializeUIButtons();
	AddUIButtons();

	/** Rest */
//	StaticMeshEditorViewport = MakeShareable((SEditorViewport*)GEditor->GetActiveViewport());
}

// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
// we call this function before unloading the module.
void FSlicingEditorModule::ShutdownModule()
{
	FSlicingEditorStyle::Shutdown();

	FSlicingEditorCommands::Unregister();
}

void FSlicingEditorModule::InitializeUIButtons()
{
	FSlicingEditorCommands::Register();

	PluginCommandList = MakeShareable(new FUICommandList);
	const FSlicingEditorCommands& Commands = FSlicingEditorCommands::Get();

	PluginCommandList->MapAction(
		Commands.OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::ShowSlicingElements),
		FCanExecuteAction());

	PluginCommandList->MapAction(
		Commands.CreateHandle,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::CreateHandle)
	);
	PluginCommandList->MapAction(
		Commands.CreateBlade,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::CreateBlade)
	);
	PluginCommandList->MapAction(
		Commands.CreateCuttingExitpoint,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::CreateCuttingExitpoint)
	);
}

void FSlicingEditorModule::AddUIButtons()
{
	IStaticMeshEditorModule& StaticMeshEditorModule =
		FModuleManager::Get().LoadModuleChecked<IStaticMeshEditorModule>("StaticMeshEditor");

	// Add menubar entry
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuBarExtension(
		"Collision",
		EExtensionHook::After,
		PluginCommandList,
		FMenuBarExtensionDelegate::CreateRaw(this, &FSlicingEditorModule::AddSlicingMenuBar)
	);
	StaticMeshEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Add toolbar entry
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Command",
		EExtensionHook::After,
		PluginCommandList,
		FToolBarExtensionDelegate::CreateRaw(this, &FSlicingEditorModule::AddSlicingToolbar)
	);
	StaticMeshEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FSlicingEditorModule::CreateHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED HANDLE"));

	RefreshViewport();
}

void FSlicingEditorModule::CreateBlade()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED BLADE"));
}

void FSlicingEditorModule::CreateCuttingExitpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED CUTTING EXITPOINT"));
}

void FSlicingEditorModule::ShowSlicingElements()
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED SLICING ELEMENTS SHOWN"));
}

static void CreateSlicingMenu(FMenuBuilder& Builder)
{
	const FSlicingEditorCommands& Commands = FSlicingEditorCommands::Get();
	
	Builder.BeginSection("SlicingAddSlicingElements");
	{
		Builder.AddMenuEntry(Commands.CreateHandle);
		Builder.AddMenuEntry(Commands.CreateBlade);
		Builder.AddMenuEntry(Commands.CreateCuttingExitpoint);
	}
	Builder.EndSection();
}

void FSlicingEditorModule::RefreshViewport()
{
	StaticMeshEditorViewport = MakeShareable((FViewport*)GEditor->GetActiveViewport());

	StaticMeshEditorViewport->Invalidate();
}

void FSlicingEditorModule::AddSlicingMenuBar(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		LOCTEXT("SlicingPluginMenu", "Slicing"),
		LOCTEXT("SlicingPluginMenu_ToolTip", "Opens a menu with commands for creating the elements needed to make the static mesh be able to slice."),
		FNewMenuDelegate::CreateStatic(CreateSlicingMenu),
		"Slicing"
	);
}

void FSlicingEditorModule::AddSlicingToolbar(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSlicingEditorCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlicingEditorModule, Slicing)