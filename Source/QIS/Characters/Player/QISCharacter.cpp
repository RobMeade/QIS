// Copyright Rob Meade. All Rights Reserved.

#include "QISCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

#include "QIS/Inventory/Components/InventoryComponent.h"
#include "QIS/Pickups/Pickup.h"


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
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
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

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

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

void AQISCharacter::PickUpItem()
{
	if (Inventory && OverlappingPickup)
	{
		// TODO: This would use the dictionary/TMap (static) having loaded in all InventoryItem/InventoryItemData assets
		UInventoryItem* InventoryItem = OverlappingPickup->GetInventoryItem().GetDefaultObject();

		if (Inventory->HasSpaceFor(InventoryItem))
		{
			// TODO: Hardcoded value of 1 at this time
			Inventory->AddToFirstEmptySlot(InventoryItem, 1);

			OverlappingPickup->Destroy();			
		}
		else
		{
			// TODO: Change the Pickup widget message accordingly
			UE_LOG(LogTemp, Warning, TEXT("Your carrying too much!"));
		}
	}
}

void AQISCharacter::SetOverlappingPickup(APickup* Pickup)
{
	if (OverlappingPickup)
	{
		OverlappingPickup->ShowPickupWidget(false);
	}

	OverlappingPickup = Pickup;

	if (OverlappingPickup)
	{
		OverlappingPickup->ShowPickupWidget(true);
	}
}
