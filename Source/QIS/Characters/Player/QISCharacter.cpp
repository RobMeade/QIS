// Copyright Rob Meade. All Rights Reserved.

#include "QISCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AQISCharacter::AQISCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AQISCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AQISCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AQISCharacter::Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AQISCharacter::StopCrouching);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AQISCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AQISCharacter::Look);		
		EnhancedInputComponent->BindAction(PickUpItemAction, ETriggerEvent::Triggered, this, &AQISCharacter::PickUpItem);		
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Triggered, this, &AQISCharacter::DropItem);		
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &AQISCharacter::UseItem);		
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &AQISCharacter::ToggleInventory);
	}
}

void AQISCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AQISCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AQISCharacter::Crouch(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Start Crouching"));
	}
}

void AQISCharacter::StopCrouching(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Stop Crouching"));
	}
}

void AQISCharacter::PickUpItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Pick Up Item"));
	}
}

void AQISCharacter::DropItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Drop Item"));
	}
}

void AQISCharacter::UseItem(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Use Item"));
	}
}

void AQISCharacter::ToggleInventory(const FInputActionValue& Value)
{
	// TODO:
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Toggle Inventory"));
	}
}
