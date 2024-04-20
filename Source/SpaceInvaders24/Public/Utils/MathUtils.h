// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/Vector2D.h"

#include "MathUtils.generated.h"

/*
 * This entire class is filled with methods that I usually use. I didn't write this class for this project, bring this from another mine projects
 */
UCLASS()
class SPACEINVADERS24_API UMathUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * AKA InverseLerp
	 * if you give me an factor0Param I return an 0.
	 * if you give me an factor1Param I return an 1.
	 * then you can give me any input and I will return a value between 0 and 1 according to the values passed before.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double LinearStep(double factor0Param, double factor1Param, double input, bool clamp) {
		const double denom = factor1Param - factor0Param;
		if (denom == 0) {
			return input < factor0Param ? 0 : 1;
		}
		const double t = (input - factor0Param) / denom;
		return clamp ? FMath::Clamp(t, 0, 1) : t;
	}

	/**
	 * if you give me an inputDataA I return an outputDataA.
	 * if you give me an inputDataB I return an outputDataB.
	 * then you can give me any input and I will return a value according to the values passed before.
	 * If clamp is true, the returns values will be held between outputDataA and outputDataB
	 *
	 * If inputDataA is 0 and inputDataB is 1, you should use lerp instead this.
	 * If outputDataA is 0 and outputDataB is 1, you should use linearstep instead this.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double RuleOfFive(double inputDataA, double outputDataA, double inputDataB, double outputDataB, double input, bool clamp) {
		const double t = LinearStep(inputDataA, inputDataB, input, clamp);
		return FMath::Lerp(outputDataA, outputDataB, t);
	}

	/**
	 * Calculates the proportional unknown value using the rule of three.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double RuleOfThree(double inputData, double outputData, double input) {
		// Avoid division by zero.
		if (outputData == 0) {
			return 0;
		}
		// Apply the rule of three and return the result.
		return (input * outputData) / inputData;
	}

	/**
	 * The function calculates the ideal uniform scale of an object based on the desired width and height on the screen.
	 * It takes into account the current screen dimensions and scaling factors for both width and height.
	 * The MinTieBreakerFunction boolean determines whether to prioritize scaling down or scaling up in cases where
	 * the object's aspect ratio doesn't match the desired screen dimensions.
	 * Set MinTieBreakerFunction to true (default) for scaling down and false for scaling up, useful for scenarios like
	 * maintaining a background image visible at all times.
	 *
	 * @param OriginalWidth - The original width of the object.
	 * @param OriginalHeight - The original height of the object.
	 * @param ScreenWidth - The current screen width.
	 * @param ScreenHeight - The current screen height.
	 * @param FactorOfScreenSizeX - The scaling factor for the width as a proportion of the screen size.
	 * @param FactorOfScreenSizeY - The scaling factor for the height as a proportion of the screen size.
	 * @param MinTieBreakerFunction - A boolean indicating whether to use the Min or Max function to determine the scaling result.
	 *                                Set to true for Min (default), false for Max.
	 * @return The calculated ideal uniform scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double GetIdealUniformScale(double OriginalWidth, double OriginalHeight, double ScreenWidth, double ScreenHeight, double FactorOfScreenSizeX, double FactorOfScreenSizeY,
									   bool MinTieBreakerFunction = true) {
		if (OriginalWidth == 0 || OriginalHeight == 0) {
			return 0;
		}

		const double AvailableSpaceX = ScreenWidth * FactorOfScreenSizeX;
		const double AvailableSpaceY = ScreenHeight * FactorOfScreenSizeY;

		const double IdealScaleX = AvailableSpaceX / OriginalWidth;
		const double IdealScaleY = AvailableSpaceY / OriginalHeight;

		if (MinTieBreakerFunction) {
			return FMath::Min(IdealScaleX, IdealScaleY);
		}
		return FMath::Max(IdealScaleX, IdealScaleY);
	}

	/**
	 * The function calculates the ideal scale in one dimension (width or height) for an object based on the desired size on the screen.
	 * It takes into account the current screen dimension and the scaling factor for the object in that dimension.
	 *
	 * @param OriginalSize - The original width or height of the object in your dimension.
	 * @param ScreenSize - The current screen width or height in your dimension.
	 * @param FactorOfScreenSize - The scaling factor for the object in your dimension as a proportion of the screen size.
	 *
	 * @return The calculated ideal scale in your dimension.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double GetIdealUniformScale1D(double OriginalSize, double ScreenSize, double FactorOfScreenSize) {
		if (OriginalSize == 0) {
			return 0;
		}
		return (ScreenSize * FactorOfScreenSize) / OriginalSize;
	}

	/**
	 * Linearly interpolates between two 2D vectors based on a given alpha value.
	 *
	 * Performs a linear interpolation (Lerp) between the vectors 'Start' and 'End' based on the alpha value.
	 *
	 * @param Start - The starting vector.
	 * @param End - The ending vector.
	 * @param Alpha - The interpolation factor. A value of 0.0 results in the 'Start' vector, 1.0 results in the 'End' vector.
	 *
	 * @return The linearly interpolated vector.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static FVector2D LerpVector2(const FVector2D &Start, const FVector2D &End, double Alpha) { return FVector2D(FMath::Lerp(Start.X, End.X, Alpha), FMath::Lerp(Start.Y, End.Y, Alpha)); }

	/**
	 * Calculates and returns the alpha value for Framerate-independent Lerp Smoothing. By Freya.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 *
	 * @param dt - DeltaTime, in seconds.
	 * @param HalfLife - Time until halfway, in seconds. You can calculate this value with the function CalculateSmoothLerpHalfLife
	 *
	 * @return Smoothed alpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double GetSmoothLerpAlpha(double dt, double HalfLife) { return 1.f - FGenericPlatformMath::Exp2(-dt / HalfLife); }

	/**
	 * Calculates the HalfLife that you need for using GetSmoothLerpAlpha for a Framerate-independent Lerp Smoothing. By Freya.
	 * We recommend you calcule this value and save, to avoid calculate the same value each frame. This uses a logarithmic function.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 * https://twitter.com/FreyaHolmer/status/1757857332187324505/photo/1
	 *
	 * @param IdealFrameRate - Framerate of the game.
	 * @param IdealLerpFactor - If your Framerate-DEPENDENT LerpSmooth function was lerp(a, b, m), is "m" the value that goes here
	 *
	 * @return HalfLife that you should use with the function GetSmoothLerpAlpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, Category = "MathUtils")
	static double CalculateSmoothLerpHalfLife(int32 IdealFrameRate, double IdealLerpFactor) { return (-1.0 / static_cast<double>(IdealFrameRate)) / FGenericPlatformMath::Log2(1.0 - IdealLerpFactor); }

	/**
	 * Checks if a given value is within a specified range.
	 *
	 * Returns true if the 'Input' value is between the 'Min' and 'Max' values (inclusive), and false otherwise.
	 *
	 * @param Input - The value to check.
	 * @param Min - The minimum value of the range.
	 * @param Max - The maximum value of the range.
	 *
	 * @return True if 'Input' is between 'Min' and 'Max', false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathUtils")
	static bool IsBetween(double Input, double Min, double Max) { return (Input >= Min && Input <= Max); }

	/**
	 * Checks if a given value is within a specified range defined by two input values.
	 *
	 * Returns true if the 'Input' value is between the minimum and maximum values of the two provided values (inclusive), and false otherwise.
	 *
	 * @param Input - The value to check.
	 * @param ValueA - The first input value.
	 * @param ValueB - The second input value.
	 *
	 * @return True if 'Input' is between the minimum and maximum values of 'ValueA' and 'ValueB', false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathUtils")
	static bool IsBetweenDynamic(double Input, double ValueA, double ValueB) {
		// Determine the minimum and maximum values.
		double MinValue = FMath::Min(ValueA, ValueB);
		double MaxValue = FMath::Max(ValueA, ValueB);

		// Check if 'Input' is between 'MinValue' and 'MaxValue'.
		return (Input >= MinValue && Input <= MaxValue);
	}

	/**
	 * Normalizes an angle in degrees to an equivalent angle within the range [0, 360).
	 *
	 * Returns the equivalent angle in the range [0, 360) for the provided angle in degrees.
	 *
	 * @param Degrees - The input angle in degrees.
	 *
	 * @return The equivalent angle within the range [0, 360).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathUtils")
	static double NormalizeDegrees(double Degrees) {
		if (Degrees >= 0) {
			return FMath::Fmod(Degrees, 360.0);
		}
		return Degrees + FMath::CeilToDouble(Degrees / -360.0) * 360.0;
	}

private:
	/**
	 * AKA InverseLerp
	 * if you give me an factor0Param I return an 0.
	 * if you give me an factor1Param I return an 1.
	 * then you can give me any input and I will return a value between 0 and 1 according to the values passed before.
	 */

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Linear Step", Category = "MathUtils", Meta = (AllowPrivateAccess, BlueprintThreadSafe))
	static double K2_LinearStep(double factor0Param, double factor1Param, double input, bool clamp) { return LinearStep(factor0Param, factor1Param, input, clamp); }

	/**
	 * if you give me an inputDataA I return an outputDataA.
	 * if you give me an inputDataB I return an outputDataB.
	 * then you can give me any input and I will return a value according to the values passed before.
	 * If clamp is true, the returns values will be held between outputDataA and outputDataB
	 *
	 * If inputDataA is 0 and inputDataB is 1, you should use lerp instead this.
	 * If outputDataA is 0 and outputDataB is 1, you should use linearstep instead this.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Rule Of Five", Category = "MathUtils", Meta = (AllowPrivateAccess, BlueprintThreadSafe))
	static double K2_RuleOfFive(double inputDataA, double outputDataA, double inputDataB, double outputDataB, double input, bool clamp) {
		return RuleOfFive(inputDataA, outputDataA, inputDataB, outputDataB, input, clamp);
	}

	/**
	 * Calculates the proportional unknown value using the rule of three.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Rule Of Three", Category = "MathUtils", Meta = (AllowPrivateAccess, BlueprintThreadSafe))
	static double K2_RuleOfThree(double inputData, double outputData, double input) { return RuleOfThree(inputData, outputData, input); }

	/**
	 * Calculates and returns the alpha value for Framerate-independent Lerp Smoothing. By Freya.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 *
	 * @param dt - DeltaTime, in seconds.
	 * @param HalfLife - Time until halfway, in seconds. You can calculate this value with the function CalculateSmoothLerpHalfLife
	 *
	 * @return Smoothed alpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Get SmoothLerp Alpha", Category = "MathUtils", Meta = (AllowPrivateAccess, BlueprintThreadSafe))
	static double K2_GetSmoothLerpAlpha(double dt, double HalfLife) { return GetSmoothLerpAlpha(dt, HalfLife); }

	/**
	 * Calculates the HalfLife that you need for using GetSmoothLerpAlpha for a Framerate-independent Lerp Smoothing. By Freya.
	 * We recommend you calcule this value and save, to avoid calculate the same value each frame. This uses a logarithmic function.
	 *
	 * https://twitter.com/FreyaHolmer/status/1757836988495847568
	 * https://twitter.com/FreyaHolmer/status/1757857332187324505/photo/1
	 *
	 * @param IdealFrameRate - Framerate of the game.
	 * @param IdealLerpFactor - If your Framerate-DEPENDENT LerpSmooth function was lerp(a, b, m), is "m" the value that goes here
	 *
	 * @return HalfLife that you should use with the function GetSmoothLerpAlpha for use with a Lerp function.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Calculate SmoothLerp HalfLife", Category = "MathUtils", Meta = (AllowPrivateAccess, BlueprintThreadSafe))
	static double K2_CalculateSmoothLerpHalfLife(int32 IdealFrameRate, double IdealLerpFactor) { return CalculateSmoothLerpHalfLife(IdealFrameRate, IdealLerpFactor); }
};
