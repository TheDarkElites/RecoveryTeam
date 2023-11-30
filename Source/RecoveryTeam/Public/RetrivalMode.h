// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RetrivalMode.generated.h"

/**
 * 
 */
UCLASS()
class RECOVERYTEAM_API ARetrivalMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARetrivalMode();
	//Our seed for generating the facility
	UPROPERTY(EditAnywhere)
	int LevelSeed{};

	//Our Level Size
	//X
	UPROPERTY(EditAnywhere)
	int LSizeX = 25;
	UPROPERTY(EditAnywhere)
	int LSizeY = 25;
	UPROPERTY(EditAnywhere)
	//This MUST be less than 0
	int LSizeZ = -1;
protected:
	virtual void BeginPlay() override;
};
