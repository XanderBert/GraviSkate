#include "GraviPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AGraviPlayer::AGraviPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AGraviPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get Mouse Position
	FVector2D mousePosition{};
	GetWorld()->GetFirstPlayerController()->GetMousePosition(mousePosition.X, mousePosition.Y);
	
	//Get Viewport Size
    FIntVector2 viewportSize{};
    GetWorld()->GetFirstPlayerController()->GetViewportSize(viewportSize.X, viewportSize.Y);

	FRotator newRotation{};
	
	newRotation.Yaw = mousePosition.X;
	newRotation.Pitch = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, viewportSize.Y), FVector2D(-90.0f, 90.0f), mousePosition.Y);
	newRotation.Roll = 0.0f;
	
	SpringArmComponent->SetWorldRotation(newRotation);
}

void AGraviPlayer::BeginPlay()
{
	Super::BeginPlay();
}