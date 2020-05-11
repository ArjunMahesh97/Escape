// Something

#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
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
	openAngle += initialYaw;

	FindPressurePlate();
	GetAudioComponent();
}

void UDoor::FindPressurePlate()
{
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

	if (TotalMass() >= massToOpen)
	{
		OpenDoor(DeltaTime);
		doorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() >= doorLastOpened + doorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}


}

void UDoor::OpenDoor(float DeltaTime)
{
	currentYaw = FMath::FInterpConstantTo(currentYaw, openAngle, DeltaTime, openSpeed);
	FRotator openDoorRotation = GetOwner()->GetActorRotation();
	openDoorRotation.Yaw = currentYaw;

	GetOwner()->SetActorRotation(openDoorRotation);

	closeDoorSound = false;
	if (!openDoorSound)
	{
		audioComponent->Play();
		openDoorSound = true;
	}
}

void UDoor::CloseDoor(float DeltaTime)
{
	currentYaw = FMath::FInterpConstantTo(currentYaw, initialYaw, DeltaTime, closeSpeed);
	FRotator openDoorRotation = GetOwner()->GetActorRotation();
	openDoorRotation.Yaw = currentYaw;

	GetOwner()->SetActorRotation(openDoorRotation);

	openDoorSound = false;
	if (!closeDoorSound)
	{
		audioComponent->Play();
		closeDoorSound = true;
	}
}

float UDoor::TotalMass() 
{
	float totalMass = 0.f;

	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors) 
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}

void UDoor::GetAudioComponent()
{
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!audioComponent) 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing audio component on %s"), *GetOwner()->GetName());
	}
}

