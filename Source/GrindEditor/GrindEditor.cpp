#include "GrindEditor.h"

#include "GrindVisualizer.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "GraviSkate/Components/GrindComponent.h"

#define LOCTEXT_NAMESPACE "FGrindEditorModule"

void FGrindEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		const TSharedPtr<FGrindVisualizer> Visualizer = MakeShareable(new FGrindVisualizer);
    
		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UGrindComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();

			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.RegisterCustomClassLayout(UGrindComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGrindComponentDetails::MakeInstance));
			
		}
	}
}
 
void FGrindEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UGrindComponent::StaticClass()->GetFName());
	}

	if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(UGrindComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGrindEditorModule, GrindEditor)