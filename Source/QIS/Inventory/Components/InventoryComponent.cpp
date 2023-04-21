// Copyright Rob Meade. All Rights Reserved.

#include "InventoryComponent.h"

#include "QIS/Inventory/Items/InventoryItemData.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Slots.Init(FInventorySlot(), InventorySize);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddToFirstEmptySlot(UInventoryItem* InventoryItem, const int32 QuantityToAdd)
{
	const int32 SlotID = FindSlot(InventoryItem);
	if (SlotID < 0)
	{
		return false;
	}

	Slots[SlotID].Item = InventoryItem;
	Slots[SlotID].Quantity += QuantityToAdd;

	UE_LOG(LogTemp, Warning, TEXT("Item added to slot ID: %s"), *FString::FromInt(SlotID));

	OnInventoryUpdated.Broadcast();

	if (GEngine)
	{
		GEngine->ClearOnScreenDebugMessages();

		for (int32 i = 0; i < Slots.Num(); i++)
		{
			if (Slots[i].Item && Slots[i].Item->GetInventoryItemData())
			{
				FString ItemName = "Item: " + Slots[i].Item->GetInventoryItemData()->GetItemName() + " (" + FString::FromInt(Slots[i].Quantity) + ")";

				GEngine->AddOnScreenDebugMessage(i, 3.f, FColor::Green, ItemName);
			}
		}		
	}

	return true;
}

bool UInventoryComponent::HasSpaceFor(const UInventoryItem* InventoryItem)
{
	return FindSlot(InventoryItem) >= 0;
}

int32 UInventoryComponent::FindSlot(const UInventoryItem* InventoryItem)
{
	int32 SlotID = FindStack(InventoryItem);
	if (SlotID < 0)
	{
		SlotID = FindEmptySlot();
	}

	UE_LOG(LogTemp, Warning, TEXT("FindSlot returned: %s"), *FString::FromInt(SlotID));

	return SlotID;
}

int32 UInventoryComponent::FindStack(const UInventoryItem* InventoryItem)
{
	if (InventoryItem->GetInventoryItemData() && !InventoryItem->GetInventoryItemData()->IsStackable())
	{
		return -1;
	}

	for (int32 SlotID = 0; SlotID < Slots.Num(); SlotID++)
	{
		//if (&Slots[SlotID].Item == &InventoryItem)
		if (Slots[SlotID].Item)
			if (Slots[SlotID].Item->GetItemID() == InventoryItem->GetItemID())
			//if (&Slots[SlotID].Item == &InventoryItem)
			{
				return SlotID;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory Items do not match, not stackable"));
			}
		else
		{
			// empty slot
		}
	}

	return -1;
}

int32 UInventoryComponent::FindEmptySlot()
{
	for (int32 SlotID = 0; SlotID < Slots.Num(); SlotID++)
	{
		if (Slots[SlotID].Item == nullptr)
		{
			return SlotID;
		}
	}

	return -1;
}
