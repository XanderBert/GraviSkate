#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GraviController.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class GRAVISKATE_API AGraviController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	
	//-------------------- Input  --------------------
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (DisplayName = "Default Mapping Context"))
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	//-------------------- Input  Functions --------------------
	void MoveForward();

	void Look();

	void Shoot();
};
