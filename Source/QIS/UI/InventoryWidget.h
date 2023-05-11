// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QIS/Inventory/Types/InventoryMoveRequest.h"

#include "InventoryWidget.generated.h"


// Forward Declarations
class UButton;
class UImage;
class UUniformGridPanel;

class UInventoryComponent;
class UInventorySlotToolTipWidget;
class UInventorySlotWidget;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryWidget_OnCloseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryWidget_OnInventoryItemMoved, FInventoryMoveRequest, InventoryMoveRequest);


UCLASS()
class QIS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	void UpdateInventory(UInventoryComponent* UpdatedInventory);

	/* Delegates */
	FInventoryWidget_OnCloseButtonClicked OnCloseButtonClicked;
	FInventoryWidget_OnInventoryItemMoved OnInventoryItemMoved;


protected:

	virtual void NativeConstruct() override;


private:

	void BindToEvents();
	void CreateInventorySlotToolTipWidget();

	UFUNCTION()
	void OnClickedCloseButton();

	UFUNCTION()
	void OnInventorySlotMoved(FInventoryMoveRequest InventoryMoveRequest);

	/*
	 * UI Widgets
	 */
	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventorySlotUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventorySlotItemUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "User Interfaces")
	TSubclassOf<UUserWidget> InventorySlotToolTipUserWidget = nullptr;

	UPROPERTY()
	UInventorySlotToolTipWidget* InventorySlotToolTipWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Background = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Close = nullptr;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel = nullptr;

};
