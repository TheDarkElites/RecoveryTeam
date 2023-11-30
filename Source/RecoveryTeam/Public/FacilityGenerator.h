// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacNode.h"
#include <stack>
#include <unordered_map>

/**
 *  Generates a facility in the passed world with the seed provided in RetrivalMode.cpp
 */
class RECOVERYTEAM_API FacilityGenerator
{
	//Ref to our world
	UWorld* WRef{};
public:
	FacilityGenerator(UWorld* v_wref);
	~FacilityGenerator();

	void GenerateFacility(std::unordered_map<FVector, AFacNode*, FVectorHash, FVectorCompare>& GenerationalMatrix);
private:
	
};
