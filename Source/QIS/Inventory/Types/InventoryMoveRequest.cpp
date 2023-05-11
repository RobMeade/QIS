// Copyright Rob Meade. All Rights Reserved.

#include "InventoryMoveRequest.h"


bool FInventoryMoveRequest::IsValidRequest() const
{
    if (FromSlotIndex > -1 && ToSlotIndex > -1)
    {
	    return true;
    }

    return false;
}
