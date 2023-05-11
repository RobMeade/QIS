// Copyright Rob Meade. All Rights Reserved.

#include "InventorySlotWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "InputCoreTypes.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"

#include "InventorySlotToolTipWidget.h"
#include "Blueprint/WidgetTree.h"
#include "QIS/Inventory/Items/InventoryItem.h"
#include "QIS/Inventory/Items/InventoryItemStaticData.h"


void UInventorySlotWidget::SetSlotItem(UInventoryItem* InventoryItem)
{
	if (InventoryItem)
	{
		bHasItem = true;

		ItemImage->SetBrushFromSoftTexture(InventoryItem->GetItemStaticData()->GetItemInventoryTexture());

		ItemQuantity->SetText(FText::AsNumber(InventoryItem->GetItemFloatStats().GetFloatStatByTag(FGameplayTag::RequestGameplayTag("Inventory.ItemFloatStat.StackSize"))));
	}
	else
	{
		bHasItem = false;

		ItemImage->SetBrushFromTexture(nullptr);
		ItemImage->ColorAndOpacity = FLinearColor::Transparent;

		ItemQuantity->SetText(FText::FromString(""));
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// apply slot highlighting
	if (SlotBackgroundImage)
	{
		SlotBackgroundImage->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 0.f, 1.f)));
		SlotBackgroundImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	// remove slot highlighting
	if (SlotBackgroundImage)
	{
		SlotBackgroundImage->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
		SlotBackgroundImage->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (bHasItem)
	{
		InventorySlotItemOverlay->SetRenderScale(FVector2D(1.25f));
		return DetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return FReply::Handled();
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (bHasItem)
	{
		InventorySlotItemOverlay->SetRenderScale(FVector2D(1.0f));		
	}

	return FReply::Handled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	SetVisibility(ESlateVisibility::HitTestInvisible);
	InventorySlotItemOverlay->SetRenderScale(FVector2D(1.25f));

	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>();
	DragDropOperation->Payload = this;
	DragDropOperation->DefaultDragVisual = InventorySlotItemOverlay;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Offset = FVector2D(-0.1f);
	OutOperation = DragDropOperation;
}

void UInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if (InOperation && InOperation->Payload)
	{
		if (const UInventorySlotWidget* InventorySlot = Cast<UInventorySlotWidget>(InOperation->Payload))
		{
			if (InventorySlot == this)
			{
				InventorySlotItemOverlay->RemoveFromParent();				
			}
		}
	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const UDragDropOperation* DragDropOperation = Cast<UDragDropOperation>(InOperation);
	if (DragDropOperation && DragDropOperation->Payload)
	{
		if (const UInventorySlotWidget* InventorySlotWidget = Cast<UInventorySlotWidget>(DragDropOperation->Payload))
		{
			OnMoved.Broadcast(FInventoryMoveRequest(InventorySlotWidget->GetSlotIndex(), SlotIndex));

			return true;
		}
	}

	return false;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InventorySlotItemOverlay->SetRenderScale(FVector2D(1.0f));

	InventorySlotOverlay->AddChild(InventorySlotItemOverlay);
}

FReply UInventorySlotWidget::DetectDrag(const FPointerEvent& PointerEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (PointerEvent.GetEffectingButton() == DragKey || PointerEvent.IsTouchEvent())
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		
		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}
