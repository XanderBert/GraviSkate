#include "GraviPlayer.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AGraviPlayer::AGraviPlayer()
{
	ProximityComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Proximity Component"));
	ProximityComponent->SetupAttachment(RootComponent);
	ProximityComponent->OnComponentBeginOverlap.AddDynamic(this, &AGraviPlayer::OnBeginOverlap);
}

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

	SetActorRotation( FMath::InterpEaseIn(GetActorRotation(), DesiredRotation, DeltaSeconds, RotationExponent));
}

void AGraviPlayer::SetDesiredRotation(const FRotator& desiredRotation)
{
	DesiredRotation = desiredRotation;
}

void AGraviPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Player -> Begin Overlap"));


	//If the overlapped Actor has a rail component we should stick to it.
	
}
