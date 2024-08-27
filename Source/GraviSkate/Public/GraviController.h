#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GraviController.generated.h"

class USpringArmComponent;
/**
 * 
 */
UCLASS()
class GRAVISKATE_API AGraviController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	USpringArmComponent* SpringArmComponent;
};
