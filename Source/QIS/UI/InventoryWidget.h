// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"


// Forward Declarations
class UButton;


// Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryWidget_OnCloseButtonClicked);


UCLASS()
class QIS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	/* Delegates */
	FInventoryWidget_OnCloseButtonClicked OnCloseButtonClicked;


protected:

	virtual void NativeConstruct() override;


private:

	void BindToEvents();

	UFUNCTION()
	void OnClickedCloseButton();

	/*
	 * UI Widgets
	 */
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Close = nullptr;
	
};
