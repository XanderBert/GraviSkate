#include "GrindVisualizer.h"
#include "GraviSkate/Components/GrindComponent.h"

IMPLEMENT_HIT_PROXY(HGrindingVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HLineProxy, HGrindingVisProxy);


void FGrindVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);

	if(const UGrindComponent* grindComp = Cast<const UGrindComponent>(Component))
	{
		for(const TPair<FVector, FVector>& Line : grindComp->Lines)
		{
			FLinearColor Color = (Line == CurrentSelectedLine) ? FLinearColor::Green : FLinearColor::Red;

			PDI->SetHitProxy(new HLineProxy(Component, Line));
			PDI->DrawLine(Line.Key, Line.Value, Color , SDPG_Foreground, 2.0f);
			PDI->SetHitProxy(nullptr);
		}
	}
}

bool FGrindVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool bEditing = false;

	if (VisProxy && VisProxy->Component.IsValid())
	{
		bEditing = true;
		GrindComponent = Cast<UGrindComponent>(const_cast<UActorComponent*>(VisProxy->Component.Get()));
		
		if (VisProxy->IsA(HLineProxy::StaticGetType()))
		{
			const HLineProxy* Proxy = static_cast<HLineProxy*>(VisProxy);
			CurrentSelectedLine = Proxy->Line;
		}
	}
	else
	{
		CurrentSelectedLine = TPair<FVector, FVector>();
	}

	return bEditing;
}

void FGrindVisualizer::EndEditing()
{
	FComponentVisualizer::EndEditing();

	CurrentSelectedLine = TPair<FVector, FVector>();
	GrindComponent.Reset();
	GEditor->RedrawLevelEditingViewports(true);
}

bool FGrindVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if(CurrentSelectedLine != TPair<FVector, FVector>())
	{
		OutLocation = (CurrentSelectedLine.Key + CurrentSelectedLine.Value) * 0.5f;
		return true;
	}
	
	return false;
}

bool FGrindVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                                        FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	bool bHandled = false;
	if(GrindComponent.IsValid())
	{
		if (CurrentSelectedLine != TPair<FVector, FVector>())
		{
			// Move the selected line
			GrindComponent->Modify();

			
			GrindComponent->Lines.Remove(CurrentSelectedLine);
			CurrentSelectedLine.Key += DeltaTranslate;
			CurrentSelectedLine.Value += DeltaTranslate;
			GrindComponent->Lines.Add(CurrentSelectedLine);

			NotifyPropertyModified(GrindComponent.Get(),	FindFProperty<FProperty>(UGrindComponent::StaticClass(),GET_MEMBER_NAME_CHECKED(UGrindComponent, Lines)));
			GEditor->RedrawLevelEditingViewports();
			
			bHandled = true;
		}
	}
			
	return bHandled;
}

bool FGrindVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	bool bHandled = false;

	if (Key == EKeys::Delete)
	{
		if(GrindComponent.IsValid() && CurrentSelectedLine != TPair<FVector, FVector>())
		{
			GrindComponent->Modify();
			GrindComponent->Lines.Remove(CurrentSelectedLine);
			CurrentSelectedLine = TPair<FVector, FVector>();
			//NotifyPropertyModified(GrindComponent.Get(),	FindFProperty<FProperty>(UGrindComponent::StaticClass(),GET_MEMBER_NAME_CHECKED(UGrindComponent, Lines)));
			GEditor->RedrawLevelEditingViewports();
			bHandled = true;
		}
	}
	
	return bHandled;
}