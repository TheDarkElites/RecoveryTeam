// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <unordered_map>
#include "FVectorCompare.h"
#include "FacNodeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RECOVERYTEAM_API UFacNodeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFacNodeComponent();
	
	// Where we can connect (North, South, East, West). This is generated upon startup.
	std::unordered_map<FVector, short, FVectorHash, FVectorCompare> Connections;
	
	// Our location in coordinates of nodes, not transform
	UPROPERTY(VisibleAnywhere)
	FVector coordinates{};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//Our connections in the form of bools, to allow to set defaults.
	UPROPERTY(EditDefaultsOnly)
	bool conNorth;
	UPROPERTY(EditDefaultsOnly)
	bool conSouth;
	UPROPERTY(EditDefaultsOnly)
	bool conEast;
	UPROPERTY(EditDefaultsOnly)
	bool conWest;
	UPROPERTY(EditDefaultsOnly)
	bool conUp;
	UPROPERTY(EditDefaultsOnly)
	bool conDown;

};
