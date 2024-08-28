﻿#include "GraviController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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

void AGraviController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputLocalPlayerSubsystem* subSystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())};
	ensureMsgf(subSystem != nullptr, TEXT("AGraviController::SetupInputComponent -> No valid enhanced input local sub system!"));

	ensureMsgf(MappingContext != nullptr, TEXT("AGraviController::SetupInputComponent -> No valid mapping context"));
	subSystem->AddMappingContext(MappingContext, 0);
	
	//Map Actions
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensureMsgf(enhancedInputComponent != nullptr, TEXT("ABasePlayerCharacter::SetupPlayerInputComponent -> No valid enhanced input component"));
	
	ensureMsgf(MoveAction != nullptr, TEXT("AGraviController::SetupInputComponent -> No valid MoveAction"));
	ensureMsgf(LookAction != nullptr, TEXT("AGraviController::SetupInputComponent -> No valid LookAction"));
	ensureMsgf(ShootAction != nullptr, TEXT("AGraviController::SetupInputComponent -> No valid ShootAction"));
	
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGraviController::MoveForward);
	enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGraviController::Look);
	enhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AGraviController::Shoot);
	
	// //Get HUD
	// AABaseHUD* hud = Cast<AABaseHUD>(playerController->GetHUD());
	// EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, hud, &AABaseHUD::ToggleInGameMenu);
}

void AGraviController::MoveForward()
{
	//Get the player
	APawn* player = GetPawn();
	ensure(player);
	
	//Get the forward vector of the player
	const FVector forwardVector = player->GetActorForwardVector();

	//Move the player
	player->AddMovementInput(forwardVector, 1.0f);
}

void AGraviController::Look()
{
	//Get Mouse Position
	FVector2D mousePosition{};
	GetMousePosition(mousePosition.X, mousePosition.Y);
	
	//Get Viewport Size
	FIntVector2 viewportSize{};
	GetViewportSize(viewportSize.X, viewportSize.Y);

	FRotator newRotation;
	
	newRotation.Yaw = mousePosition.X;
	newRotation.Pitch = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, viewportSize.Y), FVector2D(-90.0f, 90.0f), mousePosition.Y);
	newRotation.Roll = 0.0f;
	
	SpringArmComponent->SetWorldRotation(newRotation);
}

void AGraviController::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Shoot"));
}

