#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GraviPlayer.generated.h"


class USphereComponent;

UCLASS()
class GRAVISKATE_API AGraviPlayer : public ACharacter
{
	GENERATED_BODY()
	
public:
	AGraviPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void SetDesiredRotation(const FRotator& desiredRotation);
	
	UPROPERTY()
	AActor* PlayerCamera;
	
	//-------------------- Components --------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ProximityComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSoftClassPtr<AActor> CameraClass;

	//-------------------- Rotation --------------------
	FRotator DesiredRotation;

	UPROPERTY(EditAnywhere, Category = Rotation, meta=(ToolTip="The lower the slower the rotation will be."))
	float RotationExponent = 0.7f;



	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};