// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "QIS/Inventory/Types/InventoryCategory.h"

#include "InventoryItemData.generated.h"


// Forward Declarations
class UTexture2D;


UCLASS()
class QIS_API UInventoryItemData : public UDataAsset
{
	GENERATED_BODY()


public:

	FORCEINLINE FString GetItemName() const { return Name; }
	FORCEINLINE FString GetItemDescription() const { return Description; }
	FORCEINLINE bool IsStackable() const { return bIsStackable; }
	FORCEINLINE UTexture2D* GetInventorySlotTexture() const { return InventorySlotTexture; }
	FORCEINLINE EInventoryCategory GetInventoryCategory() const { return InventoryCategory; }


protected:


private:

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FString Description;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UTexture2D* InventorySlotTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EInventoryCategory InventoryCategory = EInventoryCategory::None;
};
