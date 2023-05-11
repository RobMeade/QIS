// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "Input/DragAndDrop.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "Layout/Geometry.h"

#include "QIS/Inventory/Types/InventoryMoveRequest.h"

#include "InventorySlotWidget.generated.h"


// Forward Declarations

class UDragDropOperation;
class UImage;
class UOverlay;
class UTextBlock;
class UWidget;

class UInventoryItem;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotWidget_OnMoved, FInventoryMoveRequest, InventoryMoveRequest);


UCLASS()
class QIS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	 
	void SetSlotItem(UInventoryItem* InventoryItem);

	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(const int32 Index) { SlotIndex = Index; }

	/* Delegates */
	FInventorySlotWidget_OnMoved OnMoved;


protected:
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	FReply DetectDrag(const FPointerEvent& PointerEvent, UWidget* WidgetDetectingDrag, FKey DragKey);


private:

	int32 SlotIndex = -1;
	bool bHasItem = false;

	/*
	 * UI Widgets
	 */
	UPROPERTY(meta = (BindWidget))
	UOverlay* InventorySlotOverlay = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* SlotBackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UOverlay* InventorySlotItemOverlay = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity = nullptr;
};
