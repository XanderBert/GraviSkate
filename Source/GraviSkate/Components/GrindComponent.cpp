#include "GrindComponent.h"

#include "Framework/Notifications/NotificationManager.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/ConvexElem.h"
#include "Widgets/Notifications/SNotificationList.h"


UGrindComponent::UGrindComponent()
	: PositionTolerance(15.0f)
	, PossibleGrindablePoints()
	, LineIndices()
	, StaticMeshComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrindComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	UStaticMeshComponent* staticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (!staticMeshComponent)
	{
#if WITH_EDITOR
		FNotificationInfo Info(FText::FromString("GrindComponent: No StaticMeshComponent found on owner actor"));
		Info.ExpireDuration = 5.0f;
		Info.Image = FCoreStyle::Get().GetBrush(TEXT("icons.Warning"));
		FSlateNotificationManager::Get().AddNotification(Info);
#endif
		return;
	}
	
	SetupAttachment(staticMeshComponent);
	StaticMeshComponent = staticMeshComponent;
}


void UGrindComponent::CalculatePossibleGrindableEdges()
{
	if (!StaticMeshComponent) return;
	
	// Get the BodySetup which contains collision data
	const UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
	if (!StaticMesh) return;

	UBodySetup* BodySetup = StaticMesh->GetBodySetup();
	if (!BodySetup) return;

	PossibleGrindablePoints.Empty();
	LineIndices.Empty();
	
	
	TMap<TPair<int32, int32>, TArray<int32>> EdgesArray;
	for (const FKConvexElem& ConvexElem : BodySetup->AggGeom.ConvexElems)
	{
		const TArray<FVector>& Vertices = ConvexElem.VertexData;
		// Find the edges of the convex hull
		for(auto& vertex: Vertices)
		{
			//Check if there is another point in almost the same position
			bool bIsSimilarEdge = false;
			
			for (const auto& Point : PossibleGrindablePoints)
			{
				bIsSimilarEdge = FVector::Dist(vertex, Point) < PositionTolerance;
				if(bIsSimilarEdge) break;
			}

			if(!bIsSimilarEdge) PossibleGrindablePoints.AddUnique(vertex);
		}
		
		PossibleGrindablePoints.Shrink();
	}
}
