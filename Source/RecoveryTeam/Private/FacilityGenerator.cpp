// Fill out your copyright notice in the Description page of Project Settings.

#include "FacilityGenerator.h"
#include "SeedGenerator.h"
#include "RetrivalMode.h"
#include "FacNode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include <unordered_map>
#include <tuple>
#include <chrono>
#include <string>
#define MCON 0
#define YCON 1
#define NCON 2
#define FacNodeSize 1400

using namespace std::chrono;

FacilityGenerator::FacilityGenerator(UWorld* v_wref)
{
	WRef = v_wref;
}

FacilityGenerator::~FacilityGenerator()
{
	WRef = nullptr;
}

void FacilityGenerator::GenerateFacility(std::unordered_map<FVector, AFacNode*, FVectorHash, FVectorCompare>& GenerationalMatrix)
{
	//Defines Neccesary Things
	SeedGenerator SeedHandler = SeedGenerator(WRef);
	ARetrivalMode* GameMode = Cast<ARetrivalMode>(UGameplayStatics::GetGameMode(WRef));
	//Our generational stack
	std::stack<AFacNode*> GenerationalStack;

	//Spawn Our Entrance
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	AFacNode* Starter = WRef->SpawnActor<AFacNode>(FVector(0, 0, 0), Rotation, SpawnInfo);
	Starter->coordinates = FVector(0, 0, -1);
	Starter->Connections = { {FVector::ForwardVector,YCON},{FVector::BackwardVector,NCON}, {FVector::RightVector,YCON}, {FVector::LeftVector,YCON}, {FVector::UpVector,NCON}, {FVector::DownVector,NCON} };

	//Add our entrance to our stack and matrix
	GenerationalMatrix.emplace(std::pair<FVector, AFacNode*>(FVector(0,0,-1), Starter));
	GenerationalStack.push(Starter);

	auto startGenerationTime = high_resolution_clock::now();
	bool emgStop = false;

	while(!(GenerationalStack.empty() or emgStop))
	{
		AFacNode* Current = GenerationalStack.top();
		GenerationalStack.pop();
		for (const auto& dirIterator : Current->Connections)
		{
			//get our values from our connections map
			FVector direction = dirIterator.first;
			short directiontype = dirIterator.second;

			//if we already have a node in this direction or dont want to connect in this direction, we ignore it.
			if (directiontype != YCON or GenerationalMatrix.count(direction + Current->coordinates))
			{
				continue;
			}

			FVector SpawnLoc = (FVector(FacNodeSize, FacNodeSize, FacNodeSize) * direction) + Current->GetActorLocation();
			AFacNode* NewNode = WRef->SpawnActor<AFacNode>(SpawnLoc, Rotation, SpawnInfo);
			NewNode->coordinates = direction + Current->coordinates;
			
			std::unordered_map<FVector, short, FVectorHash, FVectorCompare> ExteriorConnections = { {FVector::ForwardVector,MCON},{FVector::BackwardVector,MCON}, {FVector::RightVector,MCON}, {FVector::LeftVector,MCON}, {FVector::UpVector,MCON}, {FVector::DownVector,MCON} };
			for (const auto& newConDirIterator : ExteriorConnections)
			{	
				//get our new nodes values
				FVector Ndirection = newConDirIterator.first;
				short Ndirectiontype = newConDirIterator.second;

				FVector newCoordinates = NewNode->coordinates + Ndirection;
				//Avoid spawning anything out of bounds
				if ((newCoordinates.X < 0) or (newCoordinates.X > GameMode->LSizeX) or (newCoordinates.Y < 0) or (newCoordinates.Y > GameMode->LSizeY) or (newCoordinates.Z > -1) or (newCoordinates.Z < GameMode->LSizeZ))
				{
					ExteriorConnections[Ndirection] = NCON;
					continue;
				}

				//If we have no node saved here, then that means we dont care if we connect or not
				if (!GenerationalMatrix.count(newCoordinates))
				{
					ExteriorConnections[Ndirection] = MCON;
					continue;
				}

				//If we do have a node saved here, but its pointer is invalid, we must throw an error and sanatize the generational matrix
				if (GenerationalMatrix.find(newCoordinates)->second == NULL)
				{
					ExteriorConnections[Ndirection] = MCON;
					GenerationalMatrix.erase(newCoordinates);
					UE_LOG(LogTemp, Error, TEXT("ERROR: encountered null node in generational matrix! Key: "), *newCoordinates.ToString());
					continue;
				}
				
				AFacNode* NeighborNode = GenerationalMatrix[newCoordinates];
				//Get our inverted/incoming direction connection
				int NeighborConnectionType = NeighborNode->Connections[-1 * Ndirection];
				if (NeighborConnectionType == MCON)
				{
					UE_LOG(LogTemp, Warning, TEXT("WARNING: Unset Node encountered in generation!"));
				}

				ExteriorConnections[Ndirection] = NeighborConnectionType;
			}

			NewNode->Connections = SeedHandler.GetConnectionChoice(ExteriorConnections, NewNode->coordinates);

			//Now we insert our new node into the stack and matrix

			FString DebugText = NewNode->coordinates.ToString() + ": Direction: ";
			/*for (const auto& DdirIterator : NewNode->Connections.Connections)
			{
				FVector Ddirection = DdirIterator.first;
				short Ddirectiontype = DdirIterator.second;
				DebugText += Ddirection.ToString() + "Type: " + FString(std::to_string(Ddirectiontype).c_str());
			}*/

			NewNode->DebugTextRenderComponent->SetText(FText::FromString(DebugText));

			GenerationalMatrix.emplace(std::pair<FVector, AFacNode*>(NewNode->coordinates, NewNode));
			GenerationalStack.push(NewNode);

			if (GenerationalMatrix.size() >= abs(GameMode->LSizeX * GameMode->LSizeY * GameMode->LSizeZ))
			{
				UE_LOG(LogTemp, Error, TEXT("ERROR: Generational nodes exceeded maximium possible size!"));
				emgStop = true;
				break;
			}

			/*
			if (duration_cast<minutes>(high_resolution_clock::now() - startGenerationTime).count() >= 2)
			{
				UE_LOG(LogTemp, Error, TEXT("ERROR: Generational time exceeded two minutes!"));
				emgStop = true;
				break;
			}
			*/
		}
	}

	/*
	for (const auto& MapEntry : GenerationalMatrix)
	{
		FVector Key = MapEntry.first;        // Key is the FVector
		AFacNode* NodePtr = MapEntry.second;  // NodePtr is a pointer to the AFacNode associated with the key

		// Check if the pointer is valid before accessing
		if (NodePtr)
		{
			// Output the values using Unreal Engine 5 logging
			for (const auto& dirIterator : NodePtr->Connections.Connections)
			{
				FVector direction = dirIterator.first;
				short directiontype = dirIterator.second;

				UE_LOG(LogTemp, Display, TEXT("Key: %s, DirectionType: %d, Direction: %s"),
					*Key.ToString(), directiontype, *direction.ToString());
			}
		}
		else
		{
			// Handle the case where the pointer is null if needed
			UE_LOG(LogTemp, Display, TEXT("Key: %s has a null AFacNode pointer"), *Key.ToString());
		}
	}
	*/
}

#undef MCON
#undef YCON
#undef NCON

