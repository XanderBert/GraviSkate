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
}

void AGraviPlayer::BeginPlay()
{
	Super::BeginPlay();
}