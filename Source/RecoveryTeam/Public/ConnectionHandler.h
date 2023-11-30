// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fdirection.h"
#include "FVectorCompare.h"
#include <unordered_map>

/** This class is intended as a way to hold both the possible connections to a node, and the connections a node can make.
 *
 */

class RECOVERYTEAM_API UConnectionHandler
{
public:
	UConnectionHandler(short _North = 0, short _South = 0, short _East = 0, short _West = 0, short _Up = 0, short _Down = 0);
	UConnectionHandler(std::unordered_map<FVector, short, FVectorHash, FVectorCompare> _Connections);
	~UConnectionHandler();

	TArray<UConnectionHandler> GetConnectionList();
	UConnectionHandler GetInvert();
	int GetConnectType(FVector);

	//Where can we connect? 0 = either, 1 = must connect, 2 = cannot connect
	UPROPERTY()
	std::unordered_map<FVector, short, FVectorHash, FVectorCompare> Connections;

	//Operator overload
	bool operator==(const UConnectionHandler& other) const {
		for (const auto& dirIterator : this->Connections)
		{
			FVector direction = dirIterator.first;
			short directiontype = dirIterator.second;
			if (directiontype != other.Connections.at(direction))
			{
				return false;
			}
		}
		return true;
	}
private:
	//Recursive helper for generating possible connections given our incoming connections
	void GenerateCombinationsRecursive(TArray<UConnectionHandler>& combinations, short RequiredNorth, short RequiredSouth, short RequiredEast, short RequiredWest, short RequiredUp, short RequiredDown, short currentNorth, short currentSouth, short currentEast, short currentWest, short currentUp, short currentDown);
};
