// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "SlateLiteTypes.generated.h"

/** The three chart types SlateCharts Lite can draw. */
UENUM(BlueprintType)
enum class ESlateLiteChartType : uint8
{
	Bar,
	Line,
	/** Uses the first series only: one slice per category. */
	Pie
};

/** One named row of values, one value per category. */
USTRUCT(BlueprintType)
struct SLATECHARTSLITE_API FSlateLiteSeries
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	FString Name;

	/** Leave fully transparent (alpha 0) to use the built-in palette. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	FLinearColor Color = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	TArray<float> Values;
};
