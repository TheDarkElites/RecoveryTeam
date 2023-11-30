#pragma once

#include "CoreMinimal.h"
#include "FVectorCompare.generated.h"

//Define < operator for FVectors for use in dictionaries as keys
USTRUCT()
struct FVectorCompare {
    GENERATED_BODY()
    bool operator()(const FVector& A, const FVector& B) const {
        return FVector::PointsAreNear(A, B, KINDA_SMALL_NUMBER);
    }
};

USTRUCT()
struct FVectorHash
{   
    GENERATED_BODY()
    size_t operator()(const FVector& Vec) const
    {
        // Custom hash function combining components
        return std::hash<float>()(Vec.X) ^ std::hash<float>()(Vec.Y) ^ std::hash<float>()(Vec.Z);
    }
};