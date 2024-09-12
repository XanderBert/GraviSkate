#pragma once

#include "ComponentVisualizer.h"
#include "EditorStyleSet.h"
#include "IDetailCustomization.h"


class UGrindComponent;



//
// VisProxies
//

/**Base class for clickable targeting editing proxies*/
struct HGrindingVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HGrindingVisProxy (const UActorComponent* InComponent)
	: HComponentVisProxy(InComponent, HPP_Wireframe)
	{}
};

/**Proxy for Line*/
struct HPointProxy : public HGrindingVisProxy 
{
	DECLARE_HIT_PROXY();

	HPointProxy (const UActorComponent* InComponent, uint32 InPointIndex)
	: HGrindingVisProxy (InComponent)
	, PointIndex(InPointIndex)
	{}

	uint32 PointIndex;
};


//
//Context menu commands
//

class FTargetingVisualizerCommands : public TCommands<FTargetingVisualizerCommands>
{
public:
    FTargetingVisualizerCommands();
    virtual void RegisterCommands() override;
    TSharedPtr<FUICommandInfo> Duplicate;
    TSharedPtr<FUICommandInfo> SetConnectingMode;
};



//
// Component Visualizer
//

class  FGrindVisualizer : public FComponentVisualizer
{
public:
	virtual void OnRegister() override;
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual void EndEditing() override;
	 virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	// virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	// virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual bool IsVisualizingArchetype() const override;

	bool IsConnectingMode() const { return bConnectingMode; }
	void SetConnectingMode(bool bIsConnected) { bConnectingMode = bIsConnected; }
	
	void DuplicateSelectedPoint();
private:
	UGrindComponent* GetGrindComponent() const;;
	
	uint32 CurrentSelectedPointIndex = INDEX_NONE;
	FComponentPropertyPath GrindComponentPath;

	
	
	
	/**Output log commands*/
	TSharedPtr<FUICommandList> Actions;
	bool bConnectingMode = false;
};


//
// Custom Detail Panel
//

class FGrindComponentDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};


