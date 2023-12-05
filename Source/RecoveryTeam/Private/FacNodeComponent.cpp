// Fill out your copyright notice in the Description page of Project Settings.

#include <unordered_map>
#include "FVectorCompare.h"
#include "FacNodeComponent.h"

// Sets default values for this component's properties
UFacNodeComponent::UFacNodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Connections = { {FVector::ForwardVector,short(!conNorth)+1},{FVector::BackwardVector,short(!conSouth)+1}, {FVector::RightVector,short(!conEast)+1}, {FVector::LeftVector,short(!conWest)+1}, {FVector::UpVector,short(!conUp)+1}, {FVector::DownVector,short(!conDown)+1}};
}


// Called when the game starts
void UFacNodeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFacNodeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

