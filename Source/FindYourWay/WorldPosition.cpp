// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "WorldPosition.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	FString ObjectPosition = GetOwner()->GetActorLocation().ToString();

	// printing Actor name and Actor position on output log
	UE_LOG(LogTemp, Warning, TEXT("Object %s has position %s "), *ObjectName, *ObjectPosition);

	// this prints Text in grey color
	UE_LOG(LogTemp, Display, TEXT("Just a try"));
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

