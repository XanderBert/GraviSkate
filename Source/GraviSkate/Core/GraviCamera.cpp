#include "GraviCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AGraviCamera::AGraviCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SetRootComponent(SpringArmComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}


void AGraviCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Lerp Follow the PlayerToFollow
	const FVector currentTrans = GetActorLocation();
	const FVector desiredTrans = GetOwner()->GetActorLocation();

	const FVector newTrans = FMath::InterpEaseIn(currentTrans, desiredTrans, DeltaSeconds, RotationExponent);

	SetActorLocation(newTrans);
}