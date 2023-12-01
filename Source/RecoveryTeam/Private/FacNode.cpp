// Fill out your copyright notice in the Description page of Project Settings.


#include "FacNode.h"
#define MCON 0
#define YCON 1
#define NCON 2

// Sets default values
AFacNode::AFacNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	nodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = nodeMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(*ModelPath);
	UStaticMesh* Asset = MeshAsset.Object;

	nodeMesh->SetStaticMesh(Asset);

	Connections = { {FVector::ForwardVector,MCON},{FVector::BackwardVector,MCON}, {FVector::RightVector,MCON}, {FVector::LeftVector,MCON}, {FVector::UpVector,MCON}, {FVector::DownVector,MCON} };
}

// Called when the game starts or when spawned
void AFacNode::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFacNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#undef MCON
#undef YCON
#undef NCON