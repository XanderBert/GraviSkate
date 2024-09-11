#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrindComponent.generated.h"

UCLASS(ClassGroup=(Grind), meta=(BlueprintSpawnableComponent, DisplayName = "Grind Component", BlueprintType))
class GRAVISKATE_API UGrindComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGrindComponent();

	virtual void OnComponentCreated() override;
	
	void CalculatePossibleGrindableEdges();





	
	float PositionTolerance;

	




	
	UPROPERTY(VisibleAnywhere, Category = "Grind")
	TArray<FVector> PossibleGrindablePoints;	

	UPROPERTY(VisibleAnywhere, Category = "Grind")
	TArray<uint32> LineIndices;


private:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;
};