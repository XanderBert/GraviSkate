#include "GrindVisualizer.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "UnrealEdGlobals.h"
#include "GraviSkate/Components/GrindComponent.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Editor/UnrealEdEngine.h"

IMPLEMENT_HIT_PROXY(HGrindingVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HPointProxy, HGrindingVisProxy);

#define LOCTEXT_NAMESPACE "GrindVisualizer"

FTargetingVisualizerCommands::FTargetingVisualizerCommands(): TCommands<FTargetingVisualizerCommands>
(
	"TargetingComponentVisualizer",
	LOCTEXT("TagetingComponentVisualizer", "Targeting Component Visualizer"),
	NAME_None,
	FAppStyle::GetAppStyleSetName()
)
{}

void FTargetingVisualizerCommands::RegisterCommands()
{
	UI_COMMAND(Duplicate, "Duplicate Target", "Duplicate the current target.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SetConnectingMode, "Set Connecting Mode", "Toggle to connecting mode.", EUserInterfaceActionType::ToggleButton, FInputChord());
}



void FGrindVisualizer::OnRegister()
{
	FTargetingVisualizerCommands::Register();
	Actions = MakeShareable(new FUICommandList);
	const auto& Commands = FTargetingVisualizerCommands::Get();

	Actions->MapAction(Commands.Duplicate, FExecuteAction::CreateSP(this, &FGrindVisualizer::DuplicateSelectedPoint));
	Actions->MapAction(Commands.SetConnectingMode, FExecuteAction::CreateLambda([](bool* flag) { *flag = !(*flag); }, &bConnectingMode), FCanExecuteAction(), FIsActionChecked::CreateLambda([](const bool* flag) { return *flag; }, &bConnectingMode));
}

void FGrindVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);

	const UGrindComponent* grindComp = Cast<const UGrindComponent>(Component);
	if(!grindComp) return;

	FVector OwnerLocation = grindComp->GetOwner()->GetActorLocation();
	
	
	for(int32 i{}; i < grindComp->PossibleGrindablePoints.Num(); ++i)
	{
		FLinearColor Color = (i == CurrentSelectedPointIndex) ? FLinearColor::Green : FLinearColor::Red;

		PDI->SetHitProxy(new HPointProxy(Component, i));
		PDI->DrawPoint(grindComp->PossibleGrindablePoints[i] + OwnerLocation, Color, 20.0f, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);
	}
	
	for(int32 j{}; j < grindComp->LineIndices.Num() - 1; j+=2)
	{
		if(!grindComp->PossibleGrindablePoints.IsValidIndex(grindComp->LineIndices[j])) continue;
		if(!grindComp->PossibleGrindablePoints.IsValidIndex(grindComp->LineIndices[j+1])) continue;
		
		const FVector& Start = grindComp->PossibleGrindablePoints[grindComp->LineIndices[j]] + OwnerLocation;
		const FVector& End = grindComp->PossibleGrindablePoints[grindComp->LineIndices[j+1]] + OwnerLocation;
	
		PDI->SetHitProxy(new HGrindingVisProxy(Component));			
		PDI->DrawLine(Start, End, FLinearColor::Yellow, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);
	}
	
}

bool FGrindVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool bEditing = false;

	if (VisProxy && VisProxy->Component.IsValid())
	{
		bEditing = true;
		
		GrindComponentPath.Reset();
		GrindComponentPath = FComponentPropertyPath(VisProxy->Component.Get());
		
		if (const HPointProxy* pointProxy = HitProxyCast<HPointProxy>(VisProxy))
		{
			//If we are in connecting mode, we need to connect the points, The connections are stored as indices in the grind component
			if(bConnectingMode)
			{
				UGrindComponent* GrindComponent = GetGrindComponent();
				if(CurrentSelectedPointIndex != INDEX_NONE)
				{
					GrindComponent->Modify();
					GrindComponent->LineIndices.Add(CurrentSelectedPointIndex);
					GrindComponent->LineIndices.Add(pointProxy->PointIndex);
					CurrentSelectedPointIndex = INDEX_NONE;
				}		
			}

			CurrentSelectedPointIndex = pointProxy->PointIndex;
		}

		return bEditing;
	}

	CurrentSelectedPointIndex = INDEX_NONE;
	return bEditing;
}

void FGrindVisualizer::EndEditing()
{
	FComponentVisualizer::EndEditing();

	CurrentSelectedPointIndex = INDEX_NONE;
	GrindComponentPath.Reset();
	GEditor->RedrawLevelEditingViewports(true);
}

bool FGrindVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{ 
	if(CurrentSelectedPointIndex == INDEX_NONE) return false;

	
	OutLocation = GetGrindComponent()->PossibleGrindablePoints[CurrentSelectedPointIndex];
	return true;
}

bool FGrindVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	bool bHandled = false;
	
	if(UGrindComponent* GrindComponent = GetGrindComponent())
	{
		if(CurrentSelectedPointIndex == INDEX_NONE) return false;
		
		// Move the selected line
		GrindComponent->Modify();
		GetGrindComponent()->PossibleGrindablePoints[CurrentSelectedPointIndex] += DeltaTranslate;
		GEditor->RedrawLevelEditingViewports();
		
		bHandled = true;
	}
			
	return bHandled;
}

bool FGrindVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	bool bHandled = false;

	if (Key == EKeys::Delete)
	{
		if(CurrentSelectedPointIndex == INDEX_NONE) return false;
		
		GetGrindComponent()->Modify();
		GetGrindComponent()->PossibleGrindablePoints.RemoveAt(CurrentSelectedPointIndex);
		CurrentSelectedPointIndex = INDEX_NONE;
		GEditor->RedrawLevelEditingViewports();
		bHandled = true;
		
	}
	
	return bHandled;
}

TSharedPtr<SWidget> FGrindVisualizer::GenerateContextMenu() const
{
	FMenuBuilder MenuBuilder(true, Actions);
	{
		MenuBuilder.BeginSection("Target Actions");
		{
			MenuBuilder.AddMenuEntry(FTargetingVisualizerCommands::Get().Duplicate);
			MenuBuilder.AddMenuEntry(FTargetingVisualizerCommands::Get().SetConnectingMode);
		}
		MenuBuilder.EndSection();
	}

	TSharedPtr<SWidget> MenuWidget = MenuBuilder.MakeWidget();
	return MenuWidget;
}

bool FGrindVisualizer::IsVisualizingArchetype() const
{
	return CurrentSelectedPointIndex != INDEX_NONE;
}

void FGrindVisualizer::DuplicateSelectedPoint()
{
	if(CurrentSelectedPointIndex == INDEX_NONE) return;
	
	UGrindComponent* GrindComponent = GetGrindComponent();
	GrindComponent->Modify();

	const FVector NewPoint = GrindComponent->PossibleGrindablePoints[CurrentSelectedPointIndex];
	GrindComponent->PossibleGrindablePoints.Add(NewPoint);
	GEditor->RedrawLevelEditingViewports();
}


UGrindComponent* FGrindVisualizer::GetGrindComponent() const
{
	return Cast<UGrindComponent>(GrindComponentPath.GetComponent());
}

TSharedRef<IDetailCustomization> FGrindComponentDetails::MakeInstance()
{
	return MakeShareable(new FGrindComponentDetails);
}

void FGrindComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//Hide All Categories
	DetailBuilder.HideCategory("Variable");
	DetailBuilder.HideCategory("Sockets");
	DetailBuilder.HideCategory("Rendering");
	DetailBuilder.HideCategory("Activation");
	DetailBuilder.HideCategory("Cooking");
	DetailBuilder.HideCategory("Events");
	DetailBuilder.HideCategory("Physics");
	DetailBuilder.HideCategory("LOD");
	DetailBuilder.HideCategory("Replication");
	DetailBuilder.HideCategory("Navigation");
	DetailBuilder.HideCategory("Tags");
	
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("Grind", LOCTEXT("Grind Component", "Grind Component"), ECategoryPriority::Variable);
	
	MyCategory.AddCustomRow(FText::FromString("Generate Grindable Edges"))
    .NameContent()
    [
	    SNew(STextBlock)
	    .Text(LOCTEXT("Generate", "Generate Grindable Edges"))
	    .Font(IDetailLayoutBuilder::GetDetailFont())
    ]
    .ValueContent().MinDesiredWidth(500)
    [
	 	//Horizontal Box to hold the button and the numeric entry
	 	SNew(SHorizontalBox)
	 	+SHorizontalBox::Slot()
	 	.VAlign(VAlign_Center)
	 	[
	 		//Aligtn the text to the right and center it
	 		SNew(STextBlock).Text(LOCTEXT("Position", "Position Tolerance")).Font(IDetailLayoutBuilder::GetDetailFont()).Justification(ETextJustify::Right)
	 	]
	 	+SHorizontalBox::Slot().Padding(1.0f)
	 	[
	 		//Numeric Entry Box to set the position tolerance
	 		SNew(SNumericEntryBox<float>).Value_Lambda([&DetailBuilder]()
	 		{
	 			const UGrindComponent* GrindComponent = Cast<UGrindComponent>(DetailBuilder.GetDetailsView()->GetSelectedObjects()[0]);
	 			return GrindComponent->PositionTolerance;
	 		}).OnValueCommitted_Lambda([&DetailBuilder](float NewValue, ETextCommit::Type CommitType)
	 		{
	 			UGrindComponent* GrindComponent = Cast<UGrindComponent>(DetailBuilder.GetDetailsView()->GetSelectedObjects()[0]);
	 			GrindComponent->PositionTolerance = NewValue;
	 		}).MinValue(0.0f).MaxValue(100.0f).AllowSpin(true)
	 	]
	 	+SHorizontalBox::Slot().Padding(5.0f)
		 [
			 //Button to generate the grindable edges
			 SNew(SButton)
			 .Text(LOCTEXT("Generate", "Generate"))
			 .OnClicked_Lambda([&DetailBuilder]()
			 {
			 	UGrindComponent* GrindComponent = Cast<UGrindComponent>(DetailBuilder.GetDetailsView()->GetSelectedObjects()[0]);
			 	GrindComponent->CalculatePossibleGrindableEdges();

			 	
			 	return FReply::Handled();
			 	
			 }).ContentPadding(FMargin(10.0f)).HAlign(HAlign_Center).VAlign(VAlign_Center)
	 	]
    ];


	MyCategory.AddCustomRow(FText::FromString("Connecting Mode"))
	.NameContent()
	[
		SNew(STextBlock)
	   .Text(LOCTEXT("Ceck", "Connecting Mode"))
	   .Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	.MinDesiredWidth(500)
	[
		//Checkbox
		SNew(SCheckBox).IsChecked_Lambda([this]
		{
			//Get the FGrindVisualizer
			TSharedPtr<class FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(UGrindComponent::StaticClass());
			FGrindVisualizer* GrindVisualizer = static_cast<FGrindVisualizer*>(Visualizer.Get());
			if(GrindVisualizer->IsConnectingMode())
				return ECheckBoxState::Checked;


			return ECheckBoxState::Unchecked;
		}).OnCheckStateChanged_Lambda([this] (ECheckBoxState NewState)
		{
			//Get the FGrindVisualizer
			TSharedPtr<class FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(UGrindComponent::StaticClass());
			FGrindVisualizer* GrindVisualizer = static_cast<FGrindVisualizer*>(Visualizer.Get());
			GrindVisualizer->SetConnectingMode( NewState == ECheckBoxState::Checked);
		})
	];
}


#undef LOCTEXT_NAMESPACE