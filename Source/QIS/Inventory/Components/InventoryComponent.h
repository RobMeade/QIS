// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "QIS/Inventory/Types/InventorySlot.h"

#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QIS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:	

	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//bool AddItemToSlot(int32 SlotID, AInventoryItem, int32 QuantityToAdd);
	//bool AddToFirstEmptySlot(AInventoryItem, int32 QuantityToAdd);
	//void RemoveFromSlot(int32 SlotID, int QuantityToRemove);b


protected:

	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 InventorySize = 5;

	TArray<FInventorySlot> Slots;
};
