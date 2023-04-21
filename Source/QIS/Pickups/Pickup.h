// Copyright Rob Meade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QIS/Inventory/Items/InventoryItem.h"

#include "Pickup.generated.h"


// Forward Declarations
class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;

class AQISCharacter;


UCLASS()
class QIS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	

	APickup();

	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget) const;

	FORCEINLINE TSubclassOf<UInventoryItem> GetInventoryItem() const { return InventoryItem; }


protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:	

	UPROPERTY(EditAnywhere, Category = "Pickup|Components")
	USphereComponent* OverlapSphere = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pickup|Components")
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Pickup|Components")
	UWidgetComponent* PickupWidget;	

	UPROPERTY(EditAnywhere, Category = "Pickup")
	TSubclassOf<UInventoryItem> InventoryItem = nullptr;
};
