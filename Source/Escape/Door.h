// Something

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "Door.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindPressurePlate();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMass();

	void GetAudioComponent();

private:
	UPROPERTY(EditAnywhere)	float openAngle = 90.f;
	float initialYaw;
	float currentYaw;

	UPROPERTY(EditAnywhere) ATriggerVolume* pressurePlate;
	UPROPERTY(EditAnywhere) AActor* actorOpen;

	UPROPERTY(EditAnywhere) int32 openSpeed = 45;
	UPROPERTY(EditAnywhere) int32 closeSpeed = 90;
	UPROPERTY(EditAnywhere) float doorCloseDelay = 2.f;
	UPROPERTY(EditAnywhere) float massToOpen = 50.f;
	float doorLastOpened = 0.f;

	UPROPERTY() UAudioComponent* audioComponent = nullptr;

	bool openDoorSound = false;
	bool closeDoorSound = true;
};
