// Copyright Rob Meade. All Rights Reserved.

#include "QISPlayerController.h"

#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "QIS/Characters/Player/QISCharacter.h"


AQISPlayerController::AQISPlayerController()
{
	// ...
}

void AQISPlayerController::BeginPlay()
{
	Super::BeginPlay();

	QISCharacter = CastChecked<AQISCharacter>(GetPawn());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AQISPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AQISPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AQISPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AQISPlayerController::Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AQISPlayerController::StopCrouching);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AQISPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AQISPlayerController::Look);		
		EnhancedInputComponent->BindAction(PickUpItemAction, ETriggerEvent::Triggered, this, &AQISPlayerController::PickUpItem);		
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Triggered, this, &AQISPlayerController::DropItem);		
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &AQISPlayerController::UseItem);		
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &AQISPlayerController::ToggleInventory);
	}
}

void AQISPlayerController::Move(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->Move(Value);
	}
}

void AQISPlayerController::Look(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->Look(Value);
	}
}

void AQISPlayerController::Jump(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->Jump();
	}
}

void AQISPlayerController::StopJumping(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->StopJumping();
	}
}

void AQISPlayerController::Crouch(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->Crouch();
	}
}

void AQISPlayerController::StopCrouching(const FInputActionValue& Value)
{
	if (QISCharacter)
	{
		QISCharacter->UnCrouch();
	}
}

void AQISPlayerController::PickUpItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Pick Up Item"));
	}
}

void AQISPlayerController::DropItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Drop Item"));
	}
}

void AQISPlayerController::UseItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Use Item"));
	}
}

void AQISPlayerController::ToggleInventory(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Toggle Inventory"));
	}
}