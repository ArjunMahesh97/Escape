// Something

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInput();
}

void UGrabber::FindPhysicsHandle()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInput()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}


FVector UGrabber::GetPlayerReach()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);

	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}

FVector UGrabber::GetPlayerPos()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);

	return playerViewPointLocation;
}

void UGrabber::Grab()
{
	FHitResult hitResult = GetPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();

	if (hitResult.GetActor())
	{
		physicsHandle->GrabComponentAtLocation(componentToGrab, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent) 
	{	
		physicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetPhysicsBodyInReach()
{
	//DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 5);

	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(hit, GetPlayerPos(), GetPlayerReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParams);

	return hit;
}