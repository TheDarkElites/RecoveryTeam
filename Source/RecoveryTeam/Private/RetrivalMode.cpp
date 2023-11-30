// Fill out your copyright notice in the Description page of Project Settings.


#include "RetrivalMode.h"
#include "FacilityGenerator.h"

ARetrivalMode::ARetrivalMode()
{
	srand(time(nullptr));
	LevelSeed = rand() % 999999999;
}

void ARetrivalMode::BeginPlay()
{
	FacilityGenerator FacilityGen = FacilityGenerator(GetWorld());
	std::unordered_map<FVector, AFacNode*, FVectorHash, FVectorCompare> GenerationalMatrix;
	FacilityGen.GenerateFacility(GenerationalMatrix);
}
