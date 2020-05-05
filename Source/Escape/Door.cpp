// Something

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Door.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UDoor::UDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoor::BeginPlay()
{
	Super::BeginPlay();	

	initialYaw= GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw;

	if (!pressurePlate) 
	{
		UE_LOG(LogTemp, Error, TEXT("NO PRESSURE PLATE ON %s"), *GetOwner()->GetName());
	}

	actorOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate->IsOverlappingActor(actorOpen)) 
	{		
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}


}

void UDoor::OpenDoor(float DeltaTime)
{
	currentYaw = FMath::FInterpConstantTo(currentYaw, targetYaw, DeltaTime, openSpeed);
	FRotator openDoorRotation = GetOwner()->GetActorRotation();
	openDoorRotation.Yaw = currentYaw;

	GetOwner()->SetActorRotation(openDoorRotation);
}

void UDoor::CloseDoor(float DeltaTime)
{
	currentYaw = FMath::FInterpConstantTo(currentYaw, initialYaw, DeltaTime, closeSpeed);
	FRotator openDoorRotation = GetOwner()->GetActorRotation();
	openDoorRotation.Yaw = currentYaw;

	GetOwner()->SetActorRotation(openDoorRotation);
}

