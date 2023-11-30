#pragma once

#include "CoreMinimal.h"
#include "Fdirection.generated.h"

USTRUCT()
struct Fdirection
{
	GENERATED_BODY()
	//directiontype
	UPROPERTY(EditAnywhere)
	int directiontype = 0;
	//direction (vector form)
	UPROPERTY(EditAnywhere)
	FVector direction = FVector(0, 0, 0);
};