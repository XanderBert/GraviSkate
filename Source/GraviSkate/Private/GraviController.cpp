#include "GraviController.h"

#include "GraviPlayer.h"
#include "GameFramework/SpringArmComponent.h"

void AGraviController::BeginPlay()
{
	Super::BeginPlay();

	//Get the spring arm component from the player
	const AGraviPlayer* player = Cast<AGraviPlayer>(GetPawn());

	ensure(player);
	ensure(player->SpringArmComponent);
	
	SpringArmComponent = player->SpringArmComponent;
}


void AGraviController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Get Mouse Position
	FVector2D mousePosition{};
	GetMousePosition(mousePosition.X, mousePosition.Y);
	
	//Get Viewport Size
	FIntVector2 viewportSize{};
	GetViewportSize(viewportSize.X, viewportSize.Y);

	FRotator newRotation{};
	
	newRotation.Yaw = mousePosition.X;
	newRotation.Pitch = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, viewportSize.Y), FVector2D(-90.0f, 90.0f), mousePosition.Y);
	newRotation.Roll = 0.0f;
	
	SpringArmComponent->SetWorldRotation(newRotation);

}
