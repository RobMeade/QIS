// Copyright Rob Meade. All Rights Reserved.

#include "QISCharacter.h"

#include <string>

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

#include "QIS/Inventory/Components/InventoryComponent.h"
#include "QIS/Inventory/Items/InventoryItem.h"
#include "QIS/Inventory/Items/InventoryItemStaticData.h"
#include "QIS/Pickups/Pickup.h"


AQISCharacter::AQISCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	if (Inventory)
	{
		AddOwnedComponent(Inventory);
	}
}

void AQISCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindToComponentEvents();
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
	if (Inventory && OverlappingPickup && OverlappingPickup->GetInventory())
	{
		// TODO: If pickups are always singular, and then I create a PickupContainer class to hand things like Chests etc
		// I can then remove the bcanDestroyPickup, as it will always be destroyed if the item can be picked up, e.g. successful attempt
		bool bCanDestroyPickup = true;

		UInventoryComponent* PickupInventory = OverlappingPickup->GetInventory();

		// TODO: This would be better being within the component, not our here.  But I need to check for a "ToInventoryItem".
		if (PickupInventory->GetInventoryItems().Num() > 0)
		{
			// TODO: Note 0 index - pickups are going to only be single items etc
			UInventoryItem* InventoryItem = PickupInventory->GetInventoryItems()[0];

			UInventoryItem* ToInventoryItem = Inventory->GetInventoryItemWithSmallestStackByTag(InventoryItem->GetItemTag());
			if (ToInventoryItem)
			{
				const int32 QuantityToMove = InventoryItem->GetItemFloatStats().GetFloatStatByTag(FGameplayTag::RequestGameplayTag("Inventory.ItemFloatStat.StackSize"));
				FInventoryTransferRequest InventoryTransferRequest = FInventoryTransferRequest(PickupInventory, Inventory, InventoryItem, ToInventoryItem, QuantityToMove); 
				bCanDestroyPickup = Inventory->AttemptItemTransfer(InventoryTransferRequest);
			}
			else
			{
				if (Inventory->CanAddItemToInventory(InventoryItem))
				{
					FInventoryTransferRequest InventoryTransferRequest = FInventoryTransferRequest(PickupInventory, Inventory, InventoryItem);
					bCanDestroyPickup = Inventory->AttemptItemTransfer(InventoryTransferRequest);
				}
				else
				{
					// inventory is full
					bCanDestroyPickup = false;
				}
			}
		}

		if (bCanDestroyPickup)
		{
			OverlappingPickup->Destroy();			
		}
	}
}

void AQISCharacter::DropInventoryItem(int32 SlotIndex)
{
	// TODO: I think it might be better to be storing a pointer on the UI widget slot to the inventory item
	// it would save having to keep finding items by an inventory slot, and allow direct access

	// TODO: This all needs refactoring, need to decide where it would be better to place some of this.
	// not sure makes sense for the character to be creating pickups, but then, its the character that's
	// dropping the items from their inventory...
	// Revisiting this, I think it would make more sense for the InventoryItem to create the Pickup instance, it already has
	// all of its data and they are related.

	if (Inventory)
	{
		UInventoryItem* InventoryItem = Inventory->GetInventoryItemsForDisplay()[SlotIndex];

		FTransform PickupTransform;
		FVector Location = GetActorLocation();
		Location += FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
		PickupTransform.SetLocation(Location);
		PickupTransform.SetRotation(FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f).Quaternion());

		FActorSpawnParameters SpawnParameters;
	    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	    SpawnParameters.bDeferConstruction = true;
	    SpawnParameters.Owner = this;
	    APickup* PickupItem = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), PickupTransform, SpawnParameters);

		if (!PickupItem)
		{
			return;
		}

		// TODO: Need to remember that the item may have other data to pass than just the quantity etc
		// TODO: Hard coded "1" for testing purposes
	    PickupItem->SetupPickup(InventoryItem);
	    PickupItem->FinishSpawning(PickupTransform);

		PickupItem->TossPickup(GetActorForwardVector());

		// Remove Item (quantity) from Inventory (checks needed for quantity, reduce stack, or remove item etc)
		Inventory->RemoveItemFromInventory(InventoryItem);
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

void AQISCharacter::BindToComponentEvents()
{
	if (Inventory)
	{
		Inventory->OnInventoryUpdated.AddDynamic(this, &AQISCharacter::OnInventoryComponentInventoryUpdated);
	}
}

void AQISCharacter::OnInventoryComponentInventoryUpdated(UInventoryComponent* UpdatedInventory)
{
	OnInventoryUpdated.Broadcast(UpdatedInventory);
}
