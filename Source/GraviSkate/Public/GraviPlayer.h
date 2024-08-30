#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GraviPlayer.generated.h"


UCLASS()
class GRAVISKATE_API AGraviPlayer : public ACharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void SetDesiredRotation(const FRotator& desiredRotation);
	
	UPROPERTY()
	AActor* PlayerCamera;

private:
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSoftClassPtr<AActor> CameraClass;

	//-------------------- Rotation --------------------
	FRotator DesiredRotation;
};