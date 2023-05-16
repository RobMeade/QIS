// Copyright Rob Meade. All Rights Reserved.

#include "Pickup.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

#include "QIS/Characters/Player/QISCharacter.h"
#include "QIS/Inventory/Items/InventoryItem.h"
#include "QIS/Inventory/Items/InventoryItemStaticData.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh)
	{
		StaticMesh->SetupAttachment(OverlapSphere);
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetEnableGravity(true);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		StaticMesh->CanCharacterStepUpOn = ECB_No;
	}

	RootComponent = StaticMesh;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	if (OverlapSphere)
	{
		OverlapSphere->SetupAttachment(RootComponent);
		OverlapSphere->SetSphereRadius(150.f);
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		OverlapSphere->CanCharacterStepUpOn = ECB_No;
	}

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	if (PickupWidget)
	{
		PickupWidget->SetupAttachment(RootComponent);
		PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);
		PickupWidget->SetDrawAtDesiredSize(true);
		PickupWidget->CanCharacterStepUpOn = ECB_No;

		PickupWidget->SetRelativeLocation(FVector(0.f,0.f, 75.f));
	}

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	if (Inventory)
	{
		AddOwnedComponent(Inventory);
	}
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	if (OverlapSphere)
	{
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlap);
		OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OnSphereEndOverlap);
	}

	// TODO: Not sure about the following, in terms of picking up dropped items
	// PickupStaticData is only being used, at the moment, for the creation of new items in the pickups inventory
	// when spawned.  Once pickedup, and then dropped, this data will be lost at present.
	if (Inventory)
	{
		for (FPickupStaticDataEntry PickupStaticDataEntry : PickupStaticData.StaticData)
		{
			Inventory->AddNewItem(PickupStaticDataEntry.ItemTag, PickupStaticDataEntry.StaticPickupFloatStats.GetFloatStatByTag(FGameplayTag::RequestGameplayTag("Inventory.ItemFloatStat.StackSize")));
		}
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::ShowPickupWidget(const bool bShowWidget) const
{
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void APickup::SetupPickup(UInventoryItem* InventoryItem)
{
	// TODO: Better error handling here, maybe return bool etc?
	if (!InventoryItem)
	{
		return;
	}

	if (StaticMesh && InventoryItem->GetItemStaticData() && InventoryItem->GetItemStaticData()->GetPickupStaticMesh())
	{
		UStaticMesh* PickupMesh = InventoryItem->GetItemStaticData()->GetPickupStaticMesh().LoadSynchronous();
		StaticMesh->SetStaticMesh(PickupMesh);
	}

	if (PickupWidget && InventoryItem->GetItemStaticData())
	{
		PickupWidget->SetWidgetClass(InventoryItem->GetItemStaticData()->GetPickupWidgetClass());
	}

	// TODO: Ingoring quantity for now, stack size should be on InventoryItem at this point, so it will effectively be a full stack drop/pickup configuration
	// TODO: Need to validate array size here before adding items really
	if (Inventory)
	{
		// TODO: Will need consideration for non-stackable items (those with durability etc)

		// TODO: When we can pass a quantity through, try this
		// compare drop quantity to inventory item's stack size
		// if less than stack size
		//   reduce stack size on FromItem by quantity dropped
		//   create new InventoryItem
		//   add new InventoryItem to Pickup Inventory
		//   set new InventoryItem's Stack Size to quantity dropped
		// if equals stack size
		//   move item from player inventory to pickup inventory
		//   (might be able to use existing InventoryComponent's AttemptTransfer functionality for that)

		Inventory->AddItemToInventory(InventoryItem);
	}
}

void APickup::TossPickup(const FVector Direction)
{
	// TODO: Expose the force to be editable
	// TODO: As a nice to have, getting the position of the mouse and using that to influence
	// the vector here would be nice.  e.g. Drop the item on the right of the inventory, the item is
	// tossed to the right, drop it to the left, its tossed to the left etc
	float TossForce = 1000.f;
	FVector Impulse = Direction * TossForce;
	StaticMesh->AddImpulse(Impulse);
}

void APickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (AQISCharacter* QISCharacter = Cast<AQISCharacter>(OtherActor))
	{
		QISCharacter->SetOverlappingPickup(this);
	}
}

void APickup::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AQISCharacter* QISCharacter = Cast<AQISCharacter>(OtherActor))
	{
		QISCharacter->SetOverlappingPickup(nullptr);
	}
}
