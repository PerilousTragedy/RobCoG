// Copyright 2017, Institute for Artificial Intelligence

#include "SlicingEditorModule.h"
#include "SlicingEditorStyle.h"
#include "SlicingEditorCommands.h"

#include "LevelEditor.h"
#include "Editor.h"
#include "IStaticMeshEditor.h"
#include "StaticMeshEditorActions.h"
#include "StaticMeshEditorModule.h"

#include "Engine.h"

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
	CreateDebugButtons();

	FAssetEditorManager::Get().OnAssetOpenedInEditor().AddRaw(this, &FSlicingEditorModule::HandleAsset);
	/** Rest */
//	StaticMeshEditorViewport = MakeShareable((SEditorViewport*)GEditor->GetActiveViewport());
}

void FSlicingEditorModule::HandleAsset(UObject * Asset, IAssetEditorInstance *Editor)
{
	Mesh = Asset;
	Editore = Editor;
}

// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
// we call this function before unloading the module.
void FSlicingEditorModule::ShutdownModule()
{
	FSlicingEditorStyle::Shutdown();

	FSlicingEditorCommands::Unregister();
}

// Needed to create the menu-entries & buttons in the staticmesheditor
void FSlicingEditorModule::InitializeUIButtons()
{
	FSlicingEditorCommands::Register();

	PluginCommandList = MakeShareable(new FUICommandList);
	const FSlicingEditorCommands& Commands = FSlicingEditorCommands::Get();

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

	PluginCommandList->MapAction(
		Commands.ShowSlicingElements,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::ShowSlicingElements),
		FCanExecuteAction()
	);

	PluginCommandList->MapAction(
		Commands.EnableDebugConsoleOutput,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::OnEnableDebugConsoleOutput),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FSlicingEditorModule::OnIsEnableDebugConsoleOutputEnabled)
	);
	PluginCommandList->MapAction(
		Commands.EnableDebugShowPlane,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::OnEnableDebugShowPlane),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FSlicingEditorModule::OnIsEnableDebugShowPlaneEnabled)
	);
	PluginCommandList->MapAction(
		Commands.EnableDebugShowTrajectory,
		FExecuteAction::CreateRaw(this, &FSlicingEditorModule::OnEnableDebugShowTrajectory),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FSlicingEditorModule::OnIsEnableDebugShowTrajectoryEnabled)
	);
}

// Adds the neccessary menu-entries & buttons to the staticmesheditor to configure slicable objects
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

// Creates the buttons to enable debug options in the level editor toolbar
void FSlicingEditorModule::CreateDebugButtons()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		PluginCommandList,
		FToolBarExtensionDelegate::CreateRaw(this, &FSlicingEditorModule::AddDebugOptions)
	);
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FSlicingEditorModule::CreateHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATED HANDLE"));
	UBoxComponent* Handle = NewObject<UBoxComponent>();
	Handle->RegisterComponent();

	if (!Mesh)
		return;

	check(Editore->GetEditorName() == "StaticMeshEditor");
	auto * Tempy =  StaticCast<IStaticMeshEditor *>(Editore);
	{
		UStaticMesh* WorkingStaticMesh = Tempy->GetStaticMesh();

		UStaticMeshSocket* Sock = NewObject<UStaticMeshSocket>();
		Sock->SocketName = FName("Handle - Test");
		WorkingStaticMesh->Sockets.Emplace(Sock);
		Tempy->SetSelectedSocket(Sock);
		
	}
	
	//RefreshViewport();
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

void FSlicingEditorModule::OnEnableDebugConsoleOutput()
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG CONSOLE OUTPUT"));

	bEnableDebugConsoleOutput = !bEnableDebugConsoleOutput;
}

bool FSlicingEditorModule::OnIsEnableDebugConsoleOutputEnabled()
{
	return bEnableDebugConsoleOutput == true;
}

void FSlicingEditorModule::OnEnableDebugShowPlane()
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG SHOWING SLICING PLANE"));

	bEnableDebugShowPlane = !bEnableDebugShowPlane;
}

bool FSlicingEditorModule::OnIsEnableDebugShowPlaneEnabled()
{
	return bEnableDebugShowPlane == true;
}

void FSlicingEditorModule::OnEnableDebugShowTrajectory()
{
	UE_LOG(LogTemp, Warning, TEXT("TOGGLED DEBUG SHOWING SLICING TRAJECTORY"));

	bEnableDebugShowTrajectory = !bEnableDebugShowTrajectory;
}

bool FSlicingEditorModule::OnIsEnableDebugShowTrajectoryEnabled()
{
	return bEnableDebugShowTrajectory == true;
}

void FSlicingEditorModule::RefreshViewport()
{
	StaticMeshEditorViewport = MakeShareable((FViewport*)GEditor->GetActiveViewport());

	StaticMeshEditorViewport->Invalidate();
}

void FSlicingEditorModule::CreateSlicingMenu(FMenuBuilder& Builder)
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

TSharedRef<SWidget> FSlicingEditorModule::CreateDebugOptionMenu()
{
	FMenuBuilder Builder(false, PluginCommandList.ToSharedRef());

	const FSlicingEditorCommands& Commands = FSlicingEditorCommands::Get();
	Builder.BeginSection("SlicingDebugOptions");
	{
		Builder.AddMenuEntry(Commands.EnableDebugConsoleOutput);
		Builder.AddMenuEntry(Commands.EnableDebugShowPlane);
		Builder.AddMenuEntry(Commands.EnableDebugShowTrajectory);
	}
	Builder.EndSection();

	return Builder.MakeWidget();
}

void FSlicingEditorModule::AddSlicingMenuBar(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		LOCTEXT("SlicingPluginMenu", "Slicing"),
		LOCTEXT("SlicingPluginMenu_ToolTip", "Opens a menu with commands for creating the elements needed to make the static mesh be able to slice."),
		FNewMenuDelegate::CreateRaw(this, &FSlicingEditorModule::CreateSlicingMenu),
		"Slicing"
	);
}

void FSlicingEditorModule::AddSlicingToolbar(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSlicingEditorCommands::Get().ShowSlicingElements);
}

void FSlicingEditorModule::AddDebugOptions(FToolBarBuilder& Builder)
{
	FSlicingEditorCommands SlicingEditorCommands;
	
	Builder.AddComboButton(
		FUIAction(),
		FOnGetContent::CreateRaw(this, &FSlicingEditorModule::CreateDebugOptionMenu),
		LOCTEXT("SlicingDebugToolbar", "Slicing"),
		LOCTEXT("SlicingDebugToolbar_ToolTip", "Slicing plugin debug options"),
		FSlateIcon(FSlicingEditorStyle::GetStyleSetName(), "SlicingEditor.DebugOptionToolBar"),
		false
	);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlicingEditorModule, Slicing)