// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "VRPackFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FRotator UVRPackFunctionLibrary::ClampRotator(FRotator Rotator)
{
	return Rotator.Clamp();
}
