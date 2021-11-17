// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	//Debug Line
	if (!DoorWay) 
	{
		UE_LOG(LogTemp, Error, TEXT("DoorWay has not been set"));
	}

	ActorKey = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (DoorWay &&DoorWay->IsOverlappingActor(ActorKey))
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
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, 2.f * DeltaTime);
	FRotator NewRotation = GetOwner()->GetActorRotation();
	NewRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(NewRotation);
}

