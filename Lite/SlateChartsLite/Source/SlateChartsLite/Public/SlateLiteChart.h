// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SlateLiteTypes.h"
#include "SlateLiteChart.generated.h"

class SSlateLiteChart;

/** UMG wrapper for SSlateLiteChart. Appears in the UMG palette under "SlateCharts Lite". */
UCLASS(meta = (DisplayName = "Slate Lite Chart"))
class SLATECHARTSLITE_API USlateLiteChart : public UWidget
{
	GENERATED_BODY()

public:
	USlateLiteChart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	ESlateLiteChartType ChartType = ESlateLiteChartType::Bar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	FText Title;

	/** X-axis labels (bar, line) or slice names (pie). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	TArray<FString> Categories;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	TArray<FSlateLiteSeries> Series;

	/** Replace the data; the chart grows in again. */
	UFUNCTION(BlueprintCallable, Category = "SlateCharts Lite")
	void SetData(const TArray<FString>& InCategories, const TArray<FSlateLiteSeries>& InSeries);

	UFUNCTION(BlueprintCallable, Category = "SlateCharts Lite")
	void SetChartType(ESlateLiteChartType InType);

	UFUNCTION(BlueprintCallable, Category = "SlateCharts Lite")
	void SetTitle(const FText& InTitle);

	// UWidget
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<SSlateLiteChart> MyChart;
};
