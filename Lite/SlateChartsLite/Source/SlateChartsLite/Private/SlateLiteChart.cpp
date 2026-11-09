// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

#include "SlateLiteChart.h"
#include "SSlateLiteChart.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "SlateChartsLite"

IMPLEMENT_MODULE(FDefaultModuleImpl, SlateChartsLite)

USlateLiteChart::USlateLiteChart()
{
	// Sample data so the widget shows something the moment it is dropped into a UMG designer.
	Title = LOCTEXT("SampleTitle", "Monthly sales");
	Categories = { TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"), TEXT("May"), TEXT("Jun") };

	FSlateLiteSeries A;
	A.Name = TEXT("North");
	A.Values = { 120.f, 140.f, 98.f, 160.f, 172.f, 150.f };
	FSlateLiteSeries B;
	B.Name = TEXT("South");
	B.Values = { 80.f, 72.f, 101.f, 96.f, 120.f, 134.f };
	Series = { A, B };
}

TSharedRef<SWidget> USlateLiteChart::RebuildWidget()
{
	MyChart = SNew(SSlateLiteChart);
	return MyChart.ToSharedRef();
}

void USlateLiteChart::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyChart.IsValid())
	{
		MyChart->SetTitle(Title);
		MyChart->SetChartType(ChartType);
		MyChart->SetData(Categories, Series);
	}
}

void USlateLiteChart::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyChart.Reset();
}

void USlateLiteChart::SetData(const TArray<FString>& InCategories, const TArray<FSlateLiteSeries>& InSeries)
{
	Categories = InCategories;
	Series = InSeries;
	if (MyChart.IsValid()) { MyChart->SetData(Categories, Series); }
}

void USlateLiteChart::SetChartType(ESlateLiteChartType InType)
{
	ChartType = InType;
	if (MyChart.IsValid()) { MyChart->SetChartType(ChartType); }
}

void USlateLiteChart::SetTitle(const FText& InTitle)
{
	Title = InTitle;
	if (MyChart.IsValid()) { MyChart->SetTitle(Title); }
}

#if WITH_EDITOR
const FText USlateLiteChart::GetPaletteCategory()
{
	return LOCTEXT("PaletteCategory", "SlateCharts Lite");
}
#endif

#undef LOCTEXT_NAMESPACE
