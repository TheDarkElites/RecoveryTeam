// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionHandler.h"
#include <unordered_map>

UConnectionHandler::UConnectionHandler(short _North, short _South, short _East, short _West, short _Up, short _Down)
{	
	Connections.emplace(std::pair<FVector, short>(FVector::ForwardVector, _North));
	Connections.emplace(std::pair<FVector, short>(FVector::BackwardVector, _South));
	Connections.emplace(std::pair<FVector, short>(FVector::RightVector, _East));
	Connections.emplace(std::pair<FVector, short>(FVector::LeftVector, _West));
	Connections.emplace(std::pair<FVector, short>(FVector::UpVector, _Up));
	Connections.emplace(std::pair<FVector, short>(FVector::DownVector, _Down));
}

UConnectionHandler::UConnectionHandler(std::unordered_map<FVector, short, FVectorHash, FVectorCompare> _Connections)
{
	Connections = _Connections;
}

UConnectionHandler::~UConnectionHandler()
{
}

void UConnectionHandler::GenerateCombinationsRecursive(TArray<UConnectionHandler>& combinations, short RequiredNorth, short RequiredSouth, short RequiredEast, short RequiredWest, short RequiredUp, short RequiredDown, short currentNorth, short currentSouth, short currentEast, short currentWest, short currentUp, short currentDown)
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
		UConnectionHandler currentCombination(currentNorth, currentSouth, currentEast, currentWest, currentUp, currentDown);
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

TArray<UConnectionHandler> UConnectionHandler::GetConnectionList()
{
	TArray<UConnectionHandler> combinations;

	GenerateCombinationsRecursive(combinations, Connections.at(FVector::ForwardVector), Connections.at(FVector::BackwardVector), Connections.at(FVector::RightVector), Connections.at(FVector::LeftVector), Connections.at(FVector::UpVector), Connections.at(FVector::DownVector), 1, 1, 1, 1, 1, 1);

	return combinations;
}

UConnectionHandler UConnectionHandler::GetInvert()
//returns a connection handler that holds inverted connections so, south->north, west->east, etc.
{
	// Create a new UConnectionHandler object
	UConnectionHandler InvertedHandler;

	// Invert the connections and populate the new UConnectionHandler
	for (const auto& dirIterator : Connections)
	{
		FVector originalDirection = dirIterator.first;
		short originalDirType = dirIterator.second;

		// Invert the direction and add to the new UConnectionHandler
		FVector invertedDirection = -originalDirection;
		InvertedHandler.Connections[invertedDirection] = originalDirType;
	}

	return InvertedHandler;
}

int UConnectionHandler::GetConnectType(FVector inDirection)
//Gets the connection type of an incoming vector
{
	return Connections.at(inDirection * -1);
}
