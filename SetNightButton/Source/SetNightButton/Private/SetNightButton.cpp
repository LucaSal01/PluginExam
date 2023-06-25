// Copyright Epic Games, Inc. All Rights Reserved.

#include "SetNightButton.h"
#include "SetNightButtonStyle.h"
#include "SetNightButtonCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Editor/EditorEngine.h"


static const FName SetNightButtonTabName("SetNightButton");

#define LOCTEXT_NAMESPACE "FSetNightButtonModule"

void FSetNightButtonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSetNightButtonStyle::Initialize();
	FSetNightButtonStyle::ReloadTextures();

	FSetNightButtonCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSetNightButtonCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSetNightButtonModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSetNightButtonModule::RegisterMenus));
}

void FSetNightButtonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSetNightButtonStyle::Shutdown();

	FSetNightButtonCommands::Unregister();
}

void FSetNightButtonModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::FromString("Set Light-Mode to Night-mode");
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	AActor* FoundActor;

	FoundActor = FindActor(ADirectionalLight::StaticClass());
	if (FoundActor) 
	{
		ADirectionalLight* Sun = Cast<ADirectionalLight>(FoundActor);
		Sun->GetLightComponent()->SetIntensity(.5f);
	}

	FoundActor = FindActor(APostProcessVolume::StaticClass());
	
	APostProcessVolume* PPVolume;

	if (!FoundActor) 
	{
	    DialogText = FText::FromString("PostProcess Volume not Found, Creating One");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		FoundActor = AddActor(APostProcessVolume::StaticClass());
	}

	PPVolume = Cast<APostProcessVolume>(FoundActor);

	PPVolume->Settings.AutoExposureBias = -3.f;
	PPVolume->Settings.bOverride_AutoExposureBias = true;
	PPVolume->bUnbound = true;

}

void FSetNightButtonModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSetNightButtonCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSetNightButtonCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

AActor* FSetNightButtonModule::FindActor(TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> FoundActors;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World) 
	{
		UGameplayStatics::GetAllActorsOfClass(World, ActorClass, FoundActors);
		if (FoundActors.Num()>0) 
		{
			return FoundActors[0];
		}
	}
	return nullptr;
}

AActor* FSetNightButtonModule::AddActor(TSubclassOf<AActor> ActorClass)
{
	ULevel* Level = GEditor->GetEditorWorldContext().World()->GetCurrentLevel();
	return GEditor->AddActor(Level,ActorClass,FTransform());
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSetNightButtonModule, SetNightButton)