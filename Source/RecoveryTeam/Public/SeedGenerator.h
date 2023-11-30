// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>
#include "FVectorCompare.h"

/**
 * 
 */
class RECOVERYTEAM_API SeedGenerator
{
	//Ref to our world
	UWorld* WRef{};
public:
	SeedGenerator(UWorld* v_world);
	~SeedGenerator();
	//Returns a dictionary with connections and their respective connection type based on seed
	std::unordered_map<FVector, short, FVectorHash, FVectorCompare> GetConnectionChoice(std::unordered_map<FVector, short, FVectorHash, FVectorCompare> conIn, FVector coordinateIn);
private:
	//Recursive helper to generate direction combinations
	void GenerateCombinationsRecursive(TArray<std::unordered_map<FVector, short, FVectorHash, FVectorCompare>>& combinations, short RequiredNorth, short RequiredSouth, short RequiredEast, short RequiredWest, short RequiredUp, short RequiredDown, short currentNorth, short currentSouth, short currentEast, short currentWest, short currentUp, short currentDown);
};
