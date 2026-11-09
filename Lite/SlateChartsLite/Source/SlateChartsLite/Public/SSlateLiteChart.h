// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"
#include "SlateLiteTypes.h"

/**
 * A small chart drawn directly with Slate draw elements: bar, line or pie.
 * Hover a category (or slice) to read its values. New data grows in over a short animation.
 */
class SLATECHARTSLITE_API SSlateLiteChart : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SSlateLiteChart) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetChartType(ESlateLiteChartType InType);
	void SetTitle(const FText& InTitle);
	void SetData(const TArray<FString>& InCategories, const TArray<FSlateLiteSeries>& InSeries);

	/** Built-in palette color for series (or pie slice) Index. */
	static FLinearColor PaletteColor(int32 Index);

	// SWidget
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

private:
	/** Where the parts of the chart go for a given widget size. */
	struct FLayout
	{
		FVector2f Size;
		float TitleY = 0.f;
		float LegendY = 0.f;
		FSlateRect Plot;
		FVector2f PieCenter;
		float PieRadius = 0.f;
	};

	FLayout MakeLayout(const FVector2f& Size) const;
	float NiceMax() const;
	FLinearColor SeriesColor(int32 SeriesIndex) const;
	int32 HitTest(const FVector2f& Local, const FVector2f& Size) const;
	void PlayIntro();
	EActiveTimerReturnType TickIntro(double InCurrentTime, float InDeltaTime);

	void PaintAxes(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;
	void PaintBars(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;
	void PaintLines(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;
	void PaintPie(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;
	void PaintLegend(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;
	void PaintTooltip(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const;

	void DrawBox(const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer, const FVector2f& Pos, const FVector2f& Size, const FLinearColor& Color) const;
	void DrawText(const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer, const FVector2f& Pos, const FString& Text, const FSlateFontInfo& Font, const FLinearColor& Color) const;
	FVector2f Measure(const FString& Text, const FSlateFontInfo& Font) const;

	ESlateLiteChartType ChartType = ESlateLiteChartType::Bar;
	FText Title;
	TArray<FString> Categories;
	TArray<FSlateLiteSeries> Series;

	int32 HoverIndex = INDEX_NONE;
	FVector2f HoverPos = FVector2f::ZeroVector;

	/** 0..1 while the intro plays. */
	float Intro = 1.f;
	double IntroStart = 0.0;
	TSharedPtr<FActiveTimerHandle> IntroTimer;

	FSlateFontInfo TitleFont;
	FSlateFontInfo LabelFont;
	const FSlateBrush* WhiteBrush = nullptr;
};
