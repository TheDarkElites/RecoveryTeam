// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include <unordered_map>
#include "FVectorCompare.h"
#include "FacNode.generated.h"


UCLASS()
class RECOVERYTEAM_API AFacNode : public AActor
{
	GENERATED_BODY()
	// Our mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* nodeMesh{};

public:	
	// Sets default values for this actor's properties
	AFacNode();
	// Where we can connect (North, South, East, West)
	std::unordered_map<FVector, short, FVectorHash, FVectorCompare> Connections;
	// Our location in coordinates of nodes, not transform
	UPROPERTY(VisibleAnywhere)
	FVector coordinates{};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	//Our path to our model
	FString ModelPath = TEXT("StaticMesh'/Game/FacilityParts/Meshes/Planner/NodePlaceHolder.NodePlaceHolder'");
};
