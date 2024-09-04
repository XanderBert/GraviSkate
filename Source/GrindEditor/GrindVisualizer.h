﻿#pragma once

#include "ComponentVisualizer.h"

class UGrindComponent;

/**Base class for clickable targeting editing proxies*/
struct HGrindingVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HGrindingVisProxy (const UActorComponent* InComponent)
	: HComponentVisProxy(InComponent, HPP_Wireframe)
	{}
};

/**Proxy for Line*/
struct HLineProxy : public HGrindingVisProxy 
{
	DECLARE_HIT_PROXY();

	HLineProxy (const UActorComponent* InComponent, const TPair<FVector, FVector>& InLine)
	: HGrindingVisProxy (InComponent)
	, Line(InLine)
	{}

	TPair<FVector, FVector> Line;
};

class  FGrindVisualizer : public FComponentVisualizer
{
public:

	// Begin FComponentVisualizer interface

	//virtual void OnRegister() override;
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual void EndEditing() override;
	 virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	// virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	// virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	// virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	// virtual bool IsVisualizingArchetype();

	// End FComponentVisualizer interface


private:
	TPair<FVector, FVector> CurrentSelectedLine;
	TWeakObjectPtr<UGrindComponent> GrindComponent;
	
	
	/**Output log commands*/
	//TSharedPtr<FUICommandList> TargetingComponentVisualizerActions;
};


