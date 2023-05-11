// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "InventoryMoveRequest.generated.h"


USTRUCT()
struct FInventoryMoveRequest
{
	GENERATED_BODY()


public:

	FInventoryMoveRequest() {}

	FInventoryMoveRequest(int32 InFromSlotIndex, int32 InToSlotIndex) : FromSlotIndex(InFromSlotIndex), ToSlotIndex(InToSlotIndex) {};

	bool IsValidRequest() const;

	UPROPERTY()
	int32 FromSlotIndex = -1;

	UPROPERTY()
	int32 ToSlotIndex = -1;
};
