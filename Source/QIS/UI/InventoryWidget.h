// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QIS/Inventory/Types/InventoryMoveRequest.h"

#include "InventoryWidget.generated.h"


// Forward Declarations
class UButton;
class UCanvasPanel;
class UDragDropOperation;
class UImage;
class UUniformGridPanel;

class UInventoryBackdropWidget;
class UInventoryComponent;
class UInventorySlotToolTipWidget;
class UInventorySlotWidget;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryWidget_OnCloseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryWidget_OnInventoryItemMoved, FInventoryMoveRequest, InventoryMoveRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryWidget_OnInventoryItemDropped, int32, SlotIndex);


UCLASS()
class QIS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	void UpdateInventory(UInventoryComponent* UpdatedInventory);

	/* Delegates */
	FInventoryWidget_OnCloseButtonClicked OnCloseButtonClicked;
	FInventoryWidget_OnInventoryItemMoved OnInventoryItemMoved;
	FInventoryWidget_OnInventoryItemDropped OnInventoryItemDropped;


protected:

	virtual void NativeConstruct() override;


private:

	void BindToEvents();
	void CreateInventoryBackdrop();
	void CreateInventorySlotToolTipWidget();

	UFUNCTION()
	void OnClickedCloseButton();

	UFUNCTION()
	void OnInventorySlotMoved(FInventoryMoveRequest InventoryMoveRequest);

	UFUNCTION()
	void OnInventorySlotItemDropped(int32 SlotIndex);

	/*
	 * UI Widgets
	 */
	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventoryBackdropUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventorySlotUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventorySlotToolTipUserWidget = nullptr;

	UPROPERTY()
	UInventoryBackdropWidget* InventoryBackdrop = nullptr;

	UPROPERTY()
	UInventorySlotToolTipWidget* InventorySlotToolTipWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InventoryWidgetPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Close = nullptr;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel = nullptr;
};
