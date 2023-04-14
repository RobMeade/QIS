// Copyright Rob Meade. All Rights Reserved.

#include "InventoryWidget.h"

#include "Components/Button.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindToEvents();
}

void UInventoryWidget::BindToEvents()
{
	if (BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UInventoryWidget::OnClickedCloseButton);
	}
}

void UInventoryWidget::OnClickedCloseButton()
{
	OnCloseButtonClicked.Broadcast();
}
