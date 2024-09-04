#include "GrindComponent.h"

#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/ConvexElem.h"


UGrindComponent::UGrindComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrindComponent::OnRegister()
{
	Super::OnRegister();
	CalculateGrindableEdges();
}


void UGrindComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CalculateGrindableEdges();
}


void UGrindComponent::CalculateGrindableEdges()
{
	 // Get the parent actor's static mesh component
    const AActor* owner = GetOwner();
    if (!owner) return;

    const UStaticMeshComponent* staticMeshComponent = owner->FindComponentByClass<UStaticMeshComponent>();
    if (!staticMeshComponent) return;

    const UStaticMesh* staticMesh = staticMeshComponent->GetStaticMesh();
    if (!staticMesh) return;

	// Get the BodySetup which contains collision data
	UBodySetup* BodySetup = staticMesh->GetBodySetup();
	if (!BodySetup) return;


	
	TMap<TPair<int32, int32>, TArray<int32>> EdgesArray;
	for (const FKConvexElem& ConvexElem : BodySetup->AggGeom.ConvexElems)
	{
		const TArray<FVector>& Vertices = ConvexElem.VertexData;
		
		for (int32 i = 0; i < Vertices.Num(); ++i)
		{
			// Find the edges of the convex hull
			for (int32 j = i + 1; j < Vertices.Num(); ++j)
			{
				for (int32 k = j + 1; k < Vertices.Num(); ++k)
				{
					EdgesArray.FindOrAdd(TPair<int32, int32>(i, j)).Add(i);
					EdgesArray.FindOrAdd(TPair<int32, int32>(j, k)).Add(i);
					EdgesArray.FindOrAdd(TPair<int32, int32>(k, i)).Add(i);
				}
			}
		}
		
		
		Lines.Reserve(EdgesArray.Num());
		for (const auto& Edge : EdgesArray)
		{
			//Check if the Line is Horizontal with a margin of [-50 - 50] degrees
			FVector Edge1 = Vertices[Edge.Key.Key];
			FVector Edge2 = Vertices[Edge.Key.Value];
			FVector LineDirection = (Edge2 - Edge1).GetSafeNormal();
			
			
			const float DotProduct = FVector::DotProduct(LineDirection, FVector(0, 0, 1)); 
            const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct)); // Convert to degrees

			if(FMath::Abs(AngleDegrees - 90.0f) > HardEdgeThresholdDegrees) continue;
			if(Edge1.Z < BottomThreshold || Edge2.Z < BottomThreshold) continue;
			if(FVector::Dist(Edge1, Edge2) < MinEdgeLength) continue;


			//TODO Check if there is already a line with the almost same direction and position
			bool bIsSimilar = false;

			for (const auto& ExistingLine : Lines)
			{
				FVector ExistingStart = ExistingLine.Key;
				FVector ExistingEnd = ExistingLine.Value;
				FVector ExistingDirection = (ExistingEnd - ExistingStart).GetSafeNormal();

				// Check if the direction is similar
				const float DirectionDot = FVector::DotProduct(LineDirection, ExistingDirection);
				const float DirectionAngle = FMath::RadiansToDegrees(FMath::Acos(DirectionDot));
				if (FMath::Abs(DirectionAngle) > DirectionTolerance) continue;

				// Check if the positions are similar
				const float Distance1 = FVector::Dist(Edge1, ExistingStart);
				const float Distance2 = FVector::Dist(Edge2, ExistingEnd);
				const float Distance3 = FVector::Dist(Edge1, ExistingEnd);
				const float Distance4 = FVector::Dist(Edge2, ExistingStart);

				if (Distance1 < PositionTolerance && Distance2 < PositionTolerance)
				{
					bIsSimilar = true;
					break;
				}

				if (Distance3 < PositionTolerance && Distance4 < PositionTolerance)
				{
					bIsSimilar = true;
					break;
				}
			}

			// If no similar line was found, add this one
			if (!bIsSimilar)
			{
				Lines.Add(TPair<FVector, FVector>(Edge1, Edge2));
			}
		}

		Lines.Shrink();
	}
}