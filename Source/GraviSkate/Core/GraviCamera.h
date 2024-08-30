#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraviCamera.generated.h"

UCLASS()
class GRAVISKATE_API AGraviCamera : public AActor
{
	GENERATED_BODY()

public:
	AGraviCamera();
		
	virtual void Tick(float DeltaSeconds) override;

	//-------------------- Components --------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;
	
private:
	UPROPERTY(EditAnywhere, Category = Rotation, meta=(ToolTip="The lower the slower the rotation will be."))
    float RotationExponent = 0.2f;
};