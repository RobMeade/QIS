// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

#include "QIS/Inventory/Types/InventoryCategory.h"

#include "InventoryItem.generated.h"


// Forward Declarations
class UTexture2D;


UCLASS(Blueprintable)
class QIS_API UInventoryItem : public UObject
{
	GENERATED_BODY()


public:

	UInventoryItem();

	FORCEINLINE FGuid GetItemID() const { return ID; }
	FORCEINLINE FString GetItemName() const { return Name; }
	FORCEINLINE FString GetItemDescription() const { return Description; }
	FORCEINLINE bool IsStackable() const { return bIsStackable; }
	FORCEINLINE UTexture2D* GetInventorySlotTexture() const { return InventorySlotTexture; }
	FORCEINLINE EInventoryCategory GetInventoryCategory() const { return InventoryCategory; }


protected:


private:

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FGuid ID = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, Category = "Item")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Item")
	FString Description;

	UPROPERTY(EditAnywhere, Category = "Item")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, Category = "Item")
	UTexture2D* InventorySlotTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item")
	EInventoryCategory InventoryCategory = EInventoryCategory::None;

};
