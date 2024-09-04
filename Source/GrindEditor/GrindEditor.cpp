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
		}
	}
}

void FGrindEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UGrindComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGrindEditorModule, GrindEditor)