// Something


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
	targetYaw = initialYaw + 90.f;
}


// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentYaw = FMath::FInterpConstantTo(currentYaw, targetYaw, DeltaTime, 45);
	FRotator openDoorRotation = GetOwner()->GetActorRotation();
	openDoorRotation.Yaw = currentYaw;

	GetOwner()->SetActorRotation(openDoorRotation);


}

