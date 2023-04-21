// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "QIS/Inventory/Types/InventorySlot.h"

#include "InventoryComponent.generated.h"


// Delegate Declarations// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryComponent_OnInventoryUpdated);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QIS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:	

	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//bool AddItemToSlot(int32 SlotID, UInventoryItem* InventoryItem, int32 QuantityToAdd);
	bool AddToFirstEmptySlot(UInventoryItem* InventoryItem, const int32 QuantityToAdd);
	bool HasSpaceFor(const UInventoryItem* InventoryItem);
	//void RemoveFromSlot(int32 SlotID, int QuantityToRemove);b

	// Delegates
	FInventoryComponent_OnInventoryUpdated OnInventoryUpdated;


protected:

	virtual void BeginPlay() override;


private:

	int32 FindSlot(const UInventoryItem* InventoryItem);
	int32 FindStack(const UInventoryItem* InventoryItem);
	int32 FindEmptySlot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 InventorySize = 5;

	TArray<FInventorySlot> Slots;
};
