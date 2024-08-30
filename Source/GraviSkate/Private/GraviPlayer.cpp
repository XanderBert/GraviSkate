#include "GraviPlayer.h"

#include "Kismet/GameplayStatics.h"


void AGraviPlayer::BeginPlay()
{
	Super::BeginPlay();

	ensure(CameraClass.IsValid());
	
	//Spawn a Camera of type CameraClass
	UClass* LoadedCameraClass = CameraClass.LoadSynchronous();
	
	FActorSpawnParameters SpawnParams{};
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Spawn the camera actor
	PlayerCamera = GetWorld()->SpawnActor<AActor>(LoadedCameraClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if(PlayerCamera)
	{
		// Set the player controller's view target to the spawned camera
		UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(PlayerCamera);
	}
}

void AGraviPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorRotation( FMath::InterpEaseIn(GetActorRotation(), DesiredRotation, DeltaSeconds, 0.7f));
}

void AGraviPlayer::SetDesiredRotation(const FRotator& desiredRotation)
{
	DesiredRotation = desiredRotation;
}
