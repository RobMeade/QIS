// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QISCharacter.generated.h"


// Forward Declarations
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

struct FInputActionValue;


UCLASS(config=Game)
class AQISCharacter : public ACharacter
{
	GENERATED_BODY()


public:

	AQISCharacter();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }	


protected:


private:

	/*
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
};
