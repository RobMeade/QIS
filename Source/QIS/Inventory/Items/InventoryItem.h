// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

#include "InventoryItem.generated.h"


// Forward Declarations
class UInventoryItemData;


UCLASS(Blueprintable)
class QIS_API UInventoryItem : public UObject
{
	GENERATED_BODY()


public:

	UInventoryItem();

	FORCEINLINE FName GetItemID() const { return ID; }
	FORCEINLINE UInventoryItemData* GetInventoryItemData() const { return InventoryItemData; }


protected:


private:

	UPROPERTY(EditAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item")
	UInventoryItemData* InventoryItemData = nullptr;
};
