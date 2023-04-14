// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"


// Forward Declarations
class UImage;
class UTextBlock;

class UItemBase;


UCLASS()
class QIS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:


protected:


private:

	int32 ID;

	/*
	 * UI Widgets
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity = nullptr;
};
