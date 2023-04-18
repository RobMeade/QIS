// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "QIS/Inventory/Types/InventoryCategory.h"
#include "QIS/Inventory/Items/InventoryItem.h"

#include "InventorySlot.generated.h"


USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()


public:

	FInventorySlot() {};

	UPROPERTY()
	int32 ID = 0;

	UPROPERTY()
	int32 Capacity = 1;

	UPROPERTY()
	TArray<UInventoryItem*> Items;

	UPROPERTY()
	EInventoryCategory Category = EInventoryCategory::None;
};