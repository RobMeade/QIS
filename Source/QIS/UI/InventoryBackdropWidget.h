// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryBackdropWidget.generated.h"


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryBackdropWidget_OnInventorySlotItemDropped, int32, SlotIndex);


UCLASS()
class QIS_API UInventoryBackdropWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	/* Delegates */
	FInventoryBackdropWidget_OnInventorySlotItemDropped OnInventorySlotItemDropped;


protected:

	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


private:


};
