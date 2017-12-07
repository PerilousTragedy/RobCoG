// Copyright 2017, Institute for Artificial Intelligence

#include "SlicingEditorModule.h"
#include "SlicingEditorStyle.h"
#include "SlicingEditorCommands.h"
#include "LevelEditor.h"
#include "StaticMeshEditorModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName SlicingTabName("Slicing");

#define LOCTEXT_NAMESPACE "FSlicingEditorModule"

void FSlicingEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSlicingEditorStyle::Initialize();
	FSlicingEditorStyle::ReloadTextures();

	FSlicingEditorCommands::Register();
	
	PluginCommandList = MakeShareable(new FUICommandList);
	const FSlicingEditorCommands& Commands = FSlicingEditorCommands::Get();

	PluginCommandList->MapAction(
		Commands.OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::PluginButtonClicked),
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


	// Add menu entry
	/*{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSlicingEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	// Add toolbar entry
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSlicingEditorModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}*/
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlicingTabName, FOnSpawnTab::CreateRaw(this, &FSlicingEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSlicingTabTitle", "Slicing"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSlicingEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlicingEditorStyle::Shutdown();

	FSlicingEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlicingTabName);
}

TSharedRef<SDockTab> FSlicingEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSlicingEditorModule::OnSpawnPluginTab")),
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

void FSlicingEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SlicingTabName);
}

void FSlicingEditorModule::CreateHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED HANDLE"));
}

void FSlicingEditorModule::CreateBlade()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED BLADE"));
}

void FSlicingEditorModule::CreateCuttingExitpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED CUTTING EXITPOINT"));
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

void FSlicingEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSlicingEditorCommands::Get().OpenPluginWindow);
}

void FSlicingEditorModule::AddSlicingMenuBar(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		LOCTEXT("SlicingPluginMenu", "Slicing"),
		LOCTEXT("SlicingPluginMenu_ToolTip", "Opens a menu with commands for creating the elements needed to make the static mesh be able to slice."),
		FNewMenuDelegate::CreateStatic(CreateSlicingMenu),
		"Slicing"
	);
	
	//Builder.AddMenuEntry(FSlicingEditorCommands::Get().OpenPluginWindow);
}

void FSlicingEditorModule::AddSlicingToolbar(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSlicingEditorCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlicingEditorModule, Slicing)