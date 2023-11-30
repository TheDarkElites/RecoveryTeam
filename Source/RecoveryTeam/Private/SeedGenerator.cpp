// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedGenerator.h"
#include "RetrivalMode.h"
#include "Kismet/GameplayStatics.h"
#include "FVectorCompare.h"
#include <unordered_map>

SeedGenerator::SeedGenerator(UWorld* v_world)
{
	WRef = v_world;
}

SeedGenerator::~SeedGenerator()
{
	WRef = nullptr;
}

std::unordered_map<FVector, short, FVectorHash, FVectorCompare> SeedGenerator::GetConnectionChoice(std::unordered_map<FVector, short, FVectorHash, FVectorCompare> conIn, FVector coordinateIn)
//Will return a connectionHandler representing what sides the requested node should connect to
{
	ARetrivalMode* GameMode = Cast<ARetrivalMode>(UGameplayStatics::GetGameMode(WRef));
	int seed = GameMode->LevelSeed;

	//Gets our unique seed for Srand here by concatenating the values and then hasing them
	long long concatenatedValue = (static_cast<long long>(coordinateIn.X) << 32) | (static_cast<long long>(coordinateIn.Y) << 16) | static_cast<long long>(coordinateIn.Z);

	// Combine with the seed
	concatenatedValue += seed;

	// Hash the result
	std::hash<long long> hasher;
	int finalSeed = static_cast<int>(hasher(concatenatedValue));

	//Seeds our rand function with final seed, a unique hash, which was created above
	srand(finalSeed);
	//Gets our possible connections using recursion
	TArray<std::unordered_map<FVector, short, FVectorHash, FVectorCompare>> PossibleConnects;
	GenerateCombinationsRecursive(PossibleConnects, conIn[FVector::ForwardVector], conIn[FVector::BackwardVector], conIn[FVector::RightVector], conIn[FVector::LeftVector], conIn[FVector::UpVector], conIn[FVector::DownVector], 1, 1, 1, 1, 1, 1);
	//return a connection based on our random seed
	return PossibleConnects[rand() % PossibleConnects.Num()];
}

void SeedGenerator::GenerateCombinationsRecursive(TArray<std::unordered_map<FVector, short, FVectorHash, FVectorCompare>>& combinations, short RequiredNorth, short RequiredSouth, short RequiredEast, short RequiredWest, short RequiredUp, short RequiredDown, short currentNorth, short currentSouth, short currentEast, short currentWest, short currentUp, short currentDown)
{
	if (currentNorth > 2 || currentSouth > 2 || currentEast > 2 || currentWest > 2 || currentUp > 2 || currentDown > 2)
	{
		return;
	}

	// Recursively explore all combinations
	bool matchNorth = (RequiredNorth == 0) || (currentNorth == RequiredNorth);
	bool matchSouth = (RequiredSouth == 0) || (currentSouth == RequiredSouth);
	bool matchEast = (RequiredEast == 0) || (currentEast == RequiredEast);
	bool matchWest = (RequiredWest == 0) || (currentWest == RequiredWest);
	bool matchUp = (RequiredUp == 0) || (currentUp == RequiredUp);
	bool matchDown = (RequiredDown == 0) || (currentDown == RequiredDown);

	if (matchNorth && matchSouth && matchEast && matchWest && matchUp && matchDown)
	{
		// Check if the current combination already exists in the combinations array
		std::unordered_map<FVector, short, FVectorHash, FVectorCompare> currentCombination = { {FVector::ForwardVector,currentNorth}, {FVector::BackwardVector,currentSouth}, {FVector::RightVector,currentEast}, {FVector::LeftVector,currentWest}, {FVector::UpVector,currentUp}, {FVector::DownVector,currentDown} };
		if (!combinations.Contains(currentCombination))
		{
			combinations.Add(currentCombination);
		}
	}

	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth + 1, currentSouth, currentEast, currentWest, currentUp, currentDown);
	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth, currentSouth + 1, currentEast, currentWest, currentUp, currentDown);
	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth, currentSouth, currentEast + 1, currentWest, currentUp, currentDown);
	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth, currentSouth, currentEast, currentWest + 1, currentUp, currentDown);
	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth, currentSouth, currentEast, currentWest, currentUp + 1, currentDown);
	GenerateCombinationsRecursive(combinations, RequiredNorth, RequiredSouth, RequiredEast, RequiredWest, RequiredUp, RequiredDown, currentNorth, currentSouth, currentEast, currentWest, currentUp, currentDown + 1);
}
