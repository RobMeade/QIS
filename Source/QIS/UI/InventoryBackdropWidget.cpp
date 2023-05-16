// Copyright Rob Meade. All Rights Reserved.


#include "InventoryBackdropWidget.h"

#include "InventorySlotWidget.h"

#include "Blueprint/DragDropOperation.h"


void UInventoryBackdropWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UInventoryBackdropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->Payload)
	{
		UInventorySlotWidget* InventorySlot = Cast<UInventorySlotWidget>(InOperation->Payload);
		if (InventorySlot)
		{
			// Get index of dropped item
			// InventorySlot->GetSlotIndex();

			// broadcast an item has been dropped - to what though?
			OnInventorySlotItemDropped.Broadcast(InventorySlot->GetSlotIndex());

			return true;
		}
	}

	return false;
}
