// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "QISCharacter.generated.h"


// Forward Declarations
struct FInputActionValue;

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

class APickup;
class UInventoryComponent;


UCLASS(config=Game)
class AQISCharacter : public ACharacter
{
	GENERATED_BODY()


public:

	AQISCharacter();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PickUpItem();

	void SetOverlappingPickup(APickup* Pickup);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE APickup* GetOverlappingPickup() const { return OverlappingPickup; }


protected:


private:

	/*
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* Inventory;

	UPROPERTY()
	APickup* OverlappingPickup = nullptr;
};
