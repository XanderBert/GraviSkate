#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GraviPlayer.generated.h"

UCLASS()
class GRAVISKATE_API AGraviPlayer : public ACharacter
{
	GENERATED_BODY()
	
public:	

	AGraviPlayer();
	
	//Components
	//
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
};