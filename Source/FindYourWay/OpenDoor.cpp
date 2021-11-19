// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"


#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	FindDoorWayComponent();
	FindAudioComponent();
	
}

void UOpenDoor::FindDoorWayComponent() const
{
	if (!DoorWay)
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume has not been set"));
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio Component has not been set"));
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorOpenTime > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}	
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.8f * DeltaTime);
	FRotator NewRotation = GetOwner()->GetActorRotation();
	NewRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(NewRotation);

	CloseDoorSound = false;
	if (!AudioComponent) { return; }
	if (!OpenDoorsound)
	{
		AudioComponent->Play();
		UE_LOG(LogTemp, Warning, TEXT("OpenDoor plays"));
		OpenDoorsound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, 2.f * DeltaTime);
	FRotator NewRotation = GetOwner()->GetActorRotation();
	NewRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(NewRotation);

	OpenDoorsound = false;
	if (!AudioComponent) { return; }
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		UE_LOG(LogTemp, Warning, TEXT("CloseDoor plays"));
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!DoorWay) { return TotalMass; }
	TArray<AActor*> OverlappingActors;
	DoorWay->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

