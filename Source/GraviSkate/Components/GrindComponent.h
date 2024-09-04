#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrindComponent.generated.h"


UCLASS(ClassGroup=(Grind), meta=(BlueprintSpawnableComponent))
class GRAVISKATE_API UGrindComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrindComponent();

	virtual void OnRegister() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	
	
	UPROPERTY(EditAnywhere)
	float HardEdgeThresholdDegrees = 50.0f;

	UPROPERTY(EditAnywhere)
	float PositionTolerance  = 15.0f;
	
	UPROPERTY(EditAnywhere)
	float DirectionTolerance  = 15.0f;
	
	UPROPERTY(EditAnywhere)
	float BottomThreshold = 5.0f;
	
	UPROPERTY(EditAnywhere)
	float MinEdgeLength = 20.0f;
	
	TArray<TPair<FVector, FVector>> Lines;
private:
	void CalculateGrindableEdges();
};
