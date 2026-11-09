// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

#include "SSlateLiteChart.h"

#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Rendering/DrawElements.h"
#include "Rendering/SlateRenderer.h"
#include "Styling/CoreStyle.h"

namespace SlateLite
{
	const FLinearColor Card(0.010f, 0.014f, 0.024f, 1.f);
	const FLinearColor Grid(1.f, 1.f, 1.f, 0.07f);
	const FLinearColor Ink(0.90f, 0.92f, 0.96f, 1.f);
	const FLinearColor InkDim(0.45f, 0.50f, 0.58f, 1.f);
	const FLinearColor Tip(0.003f, 0.004f, 0.008f, 0.95f);

	constexpr float Pad = 16.f;
	constexpr float IntroSeconds = 0.6f;

	float EaseOut(float T) { T = FMath::Clamp(T, 0.f, 1.f); return 1.f - FMath::Pow(1.f - T, 3.f); }
}

void SSlateLiteChart::Construct(const FArguments& InArgs)
{
	TitleFont = FCoreStyle::GetDefaultFontStyle("Bold", 12);
	LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 9);
	WhiteBrush = FCoreStyle::Get().GetBrush("GenericWhiteBox");
}

FLinearColor SSlateLiteChart::PaletteColor(int32 Index)
{
	static const FLinearColor Palette[] = {
		FLinearColor::FromSRGBColor(FColor(0x4F, 0x8C, 0xFF)),
		FLinearColor::FromSRGBColor(FColor(0x22, 0xD3, 0xA6)),
		FLinearColor::FromSRGBColor(FColor(0xFF, 0xB3, 0x47)),
		FLinearColor::FromSRGBColor(FColor(0xFF, 0x6B, 0x8B)),
		FLinearColor::FromSRGBColor(FColor(0xA7, 0x8B, 0xFA)),
		FLinearColor::FromSRGBColor(FColor(0x38, 0xBD, 0xF8)),
	};
	return Palette[FMath::Abs(Index) % UE_ARRAY_COUNT(Palette)];
}

FLinearColor SSlateLiteChart::SeriesColor(int32 SeriesIndex) const
{
	if (Series.IsValidIndex(SeriesIndex) && Series[SeriesIndex].Color.A > 0.f)
	{
		return Series[SeriesIndex].Color;
	}
	return PaletteColor(SeriesIndex);
}

void SSlateLiteChart::SetChartType(ESlateLiteChartType InType)
{
	if (ChartType != InType)
	{
		ChartType = InType;
		HoverIndex = INDEX_NONE;
		PlayIntro();
	}
}

void SSlateLiteChart::SetTitle(const FText& InTitle)
{
	Title = InTitle;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSlateLiteChart::SetData(const TArray<FString>& InCategories, const TArray<FSlateLiteSeries>& InSeries)
{
	Categories = InCategories;
	Series = InSeries;
	HoverIndex = INDEX_NONE;
	PlayIntro();
}

void SSlateLiteChart::PlayIntro()
{
	Intro = 0.f;
	IntroStart = FSlateApplication::IsInitialized() ? FSlateApplication::Get().GetCurrentTime() : 0.0;
	if (!IntroTimer.IsValid())
	{
		IntroTimer = RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SSlateLiteChart::TickIntro));
	}
	Invalidate(EInvalidateWidgetReason::Paint);
}

EActiveTimerReturnType SSlateLiteChart::TickIntro(double InCurrentTime, float InDeltaTime)
{
	Intro = FMath::Clamp(static_cast<float>((InCurrentTime - IntroStart) / SlateLite::IntroSeconds), 0.f, 1.f);
	Invalidate(EInvalidateWidgetReason::Paint);
	if (Intro >= 1.f)
	{
		IntroTimer.Reset();
		return EActiveTimerReturnType::Stop;
	}
	return EActiveTimerReturnType::Continue;
}

FVector2D SSlateLiteChart::ComputeDesiredSize(float) const
{
	return FVector2D(480.0, 300.0);
}

float SSlateLiteChart::NiceMax() const
{
	float Max = 0.f;
	for (const FSlateLiteSeries& S : Series)
	{
		for (const float V : S.Values)
		{
			if (FMath::IsFinite(V)) { Max = FMath::Max(Max, V); }
		}
	}
	if (Max <= 0.f) { return 1.f; }
	// Round up to 1, 2, 2.5 or 5 times a power of ten so the four ticks read cleanly.
	const float Step = Max / 4.f;
	const float Pow10 = FMath::Pow(10.f, FMath::FloorToFloat(FMath::LogX(10.f, Step)));
	for (const float M : { 1.f, 2.f, 2.5f, 5.f, 10.f })
	{
		if (Step <= M * Pow10) { return M * Pow10 * 4.f; }
	}
	return Max;
}

SSlateLiteChart::FLayout SSlateLiteChart::MakeLayout(const FVector2f& Size) const
{
	using namespace SlateLite;
	FLayout L;
	L.Size = Size;
	L.TitleY = Pad;
	const float TitleH = Title.IsEmpty() ? 0.f : Measure(TEXT("Ag"), TitleFont).Y + 6.f;
	L.LegendY = Pad + TitleH;
	const float LegendH = Measure(TEXT("Ag"), LabelFont).Y + 10.f;
	const float Top = L.LegendY + LegendH;

	const float AxisW = ChartType == ESlateLiteChartType::Pie ? 0.f : Measure(TEXT("0000.0"), LabelFont).X + 8.f;
	const float AxisH = ChartType == ESlateLiteChartType::Pie ? 0.f : Measure(TEXT("Ag"), LabelFont).Y + 8.f;
	L.Plot = FSlateRect(Pad + AxisW, Top, FMath::Max(Pad + AxisW + 1.f, Size.X - Pad), FMath::Max(Top + 1.f, Size.Y - Pad - AxisH));

	const FVector2f PlotSize(L.Plot.Right - L.Plot.Left, L.Plot.Bottom - L.Plot.Top);
	L.PieCenter = FVector2f(L.Plot.Left + PlotSize.X * 0.5f, L.Plot.Top + PlotSize.Y * 0.5f);
	L.PieRadius = FMath::Max(4.f, FMath::Min(PlotSize.X, PlotSize.Y) * 0.5f - 4.f);
	return L;
}

int32 SSlateLiteChart::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2f Size = FVector2f(AllottedGeometry.GetLocalSize());
	const FLayout L = MakeLayout(Size);

	DrawBox(AllottedGeometry, OutDrawElements, LayerId, FVector2f::ZeroVector, Size, SlateLite::Card);
	if (!Title.IsEmpty())
	{
		DrawText(AllottedGeometry, OutDrawElements, LayerId + 3, FVector2f(SlateLite::Pad, L.TitleY), Title.ToString(), TitleFont, SlateLite::Ink);
	}
	PaintLegend(L, AllottedGeometry, OutDrawElements, LayerId + 3);

	const bool bHasData = Categories.Num() > 0 && Series.Num() > 0;
	if (!bHasData)
	{
		const FString Empty = TEXT("No data");
		const FVector2f S = Measure(Empty, LabelFont);
		DrawText(AllottedGeometry, OutDrawElements, LayerId + 3, FVector2f((Size.X - S.X) * 0.5f, (Size.Y - S.Y) * 0.5f), Empty, LabelFont, SlateLite::InkDim);
		return LayerId + 3;
	}

	switch (ChartType)
	{
	case ESlateLiteChartType::Bar:
		PaintAxes(L, AllottedGeometry, OutDrawElements, LayerId + 1);
		PaintBars(L, AllottedGeometry, OutDrawElements, LayerId + 2);
		break;
	case ESlateLiteChartType::Line:
		PaintAxes(L, AllottedGeometry, OutDrawElements, LayerId + 1);
		PaintLines(L, AllottedGeometry, OutDrawElements, LayerId + 2);
		break;
	case ESlateLiteChartType::Pie:
		PaintPie(L, AllottedGeometry, OutDrawElements, LayerId + 2);
		break;
	}

	PaintTooltip(L, AllottedGeometry, OutDrawElements, LayerId + 5);
	return LayerId + 7;
}

void SSlateLiteChart::PaintAxes(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	const float Max = NiceMax();
	const float H = L.Plot.Bottom - L.Plot.Top;
	for (int32 Tick = 0; Tick <= 4; ++Tick)
	{
		const float Y = L.Plot.Bottom - H * Tick / 4.f;
		FSlateDrawElement::MakeLines(Out, Layer, Geo.ToPaintGeometry(), TArray<FVector2f>{ FVector2f(L.Plot.Left, Y), FVector2f(L.Plot.Right, Y) },
			ESlateDrawEffect::None, SlateLite::Grid, true, 1.f);
		const FString Label = FText::AsNumber(Max * Tick / 4.f).ToString();
		const FVector2f S = Measure(Label, LabelFont);
		DrawText(Geo, Out, Layer, FVector2f(L.Plot.Left - S.X - 6.f, Y - S.Y * 0.5f), Label, LabelFont, SlateLite::InkDim);
	}

	const int32 N = Categories.Num();
	const float Band = (L.Plot.Right - L.Plot.Left) / N;
	for (int32 i = 0; i < N; ++i)
	{
		const FVector2f S = Measure(Categories[i], LabelFont);
		if (S.X > Band - 2.f && N > 1) { continue; } // too crowded: skip rather than overlap
		DrawText(Geo, Out, Layer, FVector2f(L.Plot.Left + Band * (i + 0.5f) - S.X * 0.5f, L.Plot.Bottom + 6.f), Categories[i],
			LabelFont, i == HoverIndex ? SlateLite::Ink : SlateLite::InkDim);
	}
}

void SSlateLiteChart::PaintBars(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	const float Max = NiceMax();
	const int32 N = Categories.Num();
	const int32 NS = Series.Num();
	const float Band = (L.Plot.Right - L.Plot.Left) / N;
	const float Group = Band * 0.7f;
	const float BarW = FMath::Max(1.f, Group / NS - 2.f);
	const float Grow = SlateLite::EaseOut(Intro);

	if (HoverIndex != INDEX_NONE)
	{
		DrawBox(Geo, Out, Layer - 1, FVector2f(L.Plot.Left + Band * HoverIndex, L.Plot.Top), FVector2f(Band, L.Plot.Bottom - L.Plot.Top), FLinearColor(1.f, 1.f, 1.f, 0.04f));
	}
	for (int32 s = 0; s < NS; ++s)
	{
		const FLinearColor Color = SeriesColor(s);
		for (int32 i = 0; i < N && i < Series[s].Values.Num(); ++i)
		{
			const float V = Series[s].Values[i];
			if (!FMath::IsFinite(V) || V <= 0.f) { continue; }
			const float H = (L.Plot.Bottom - L.Plot.Top) * FMath::Min(V / Max, 1.f) * Grow;
			const float X = L.Plot.Left + Band * i + (Band - Group) * 0.5f + s * (Group / NS) + 1.f;
			const bool bDim = HoverIndex != INDEX_NONE && HoverIndex != i;
			DrawBox(Geo, Out, Layer, FVector2f(X, L.Plot.Bottom - H), FVector2f(BarW, H), bDim ? Color.CopyWithNewOpacity(0.45f) : Color);
		}
	}
}

void SSlateLiteChart::PaintLines(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	const float Max = NiceMax();
	const int32 N = Categories.Num();
	const float Band = (L.Plot.Right - L.Plot.Left) / N;
	const float Grow = SlateLite::EaseOut(Intro);

	if (HoverIndex != INDEX_NONE)
	{
		const float X = L.Plot.Left + Band * (HoverIndex + 0.5f);
		FSlateDrawElement::MakeLines(Out, Layer - 1, Geo.ToPaintGeometry(), TArray<FVector2f>{ FVector2f(X, L.Plot.Top), FVector2f(X, L.Plot.Bottom) },
			ESlateDrawEffect::None, FLinearColor(1.f, 1.f, 1.f, 0.18f), true, 1.f);
	}
	for (int32 s = 0; s < Series.Num(); ++s)
	{
		const FLinearColor Color = SeriesColor(s);
		TArray<FVector2f> Points;
		for (int32 i = 0; i < N && i < Series[s].Values.Num(); ++i)
		{
			const float V = Series[s].Values[i];
			if (!FMath::IsFinite(V))
			{
				// Missing value: end this run of the line and start a new one after the gap.
				if (Points.Num() > 1) { FSlateDrawElement::MakeLines(Out, Layer, Geo.ToPaintGeometry(), Points, ESlateDrawEffect::None, Color, true, 2.f); }
				Points.Reset();
				continue;
			}
			const float Y = L.Plot.Bottom - (L.Plot.Bottom - L.Plot.Top) * FMath::Clamp(V / Max, 0.f, 1.f) * Grow;
			const FVector2f P(L.Plot.Left + Band * (i + 0.5f), Y);
			Points.Add(P);
			const float Dot = i == HoverIndex ? 8.f : 5.f;
			DrawBox(Geo, Out, Layer + 1, P - FVector2f(Dot * 0.5f), FVector2f(Dot), Color);
		}
		if (Points.Num() > 1)
		{
			FSlateDrawElement::MakeLines(Out, Layer, Geo.ToPaintGeometry(), Points, ESlateDrawEffect::None, Color, true, 2.f);
		}
	}
}

void SSlateLiteChart::PaintPie(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	const TArray<float>& Values = Series[0].Values;
	float Total = 0.f;
	for (int32 i = 0; i < Categories.Num() && i < Values.Num(); ++i)
	{
		if (FMath::IsFinite(Values[i]) && Values[i] > 0.f) { Total += Values[i]; }
	}
	if (Total <= 0.f) { return; }

	const FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*WhiteBrush);
	const FSlateRenderTransform& Xf = Geo.GetAccumulatedRenderTransform();
	const float Sweep = 2.f * PI * SlateLite::EaseOut(Intro);
	float Angle = -0.5f * PI; // start at 12 o'clock

	for (int32 i = 0; i < Categories.Num() && i < Values.Num(); ++i)
	{
		const float V = Values[i];
		if (!FMath::IsFinite(V) || V <= 0.f) { continue; }
		const float Span = Sweep * V / Total;
		const float Radius = L.PieRadius * (i == HoverIndex ? 1.f : 0.94f);
		const FColor Color = (HoverIndex != INDEX_NONE && HoverIndex != i ? PaletteColor(i).CopyWithNewOpacity(0.5f) : PaletteColor(i)).ToFColor(true);

		TArray<FSlateVertex> Verts;
		TArray<SlateIndex> Indices;
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(Xf, L.PieCenter, FVector2f(0.5f, 0.5f), Color));
		const int32 Steps = FMath::Max(2, FMath::CeilToInt(Span / (2.f * PI) * 96.f));
		for (int32 k = 0; k <= Steps; ++k)
		{
			const float A = Angle + Span * k / Steps;
			const FVector2f P = L.PieCenter + FVector2f(FMath::Cos(A), FMath::Sin(A)) * Radius;
			Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(Xf, P, FVector2f(0.5f, 0.5f), Color));
			if (k > 0)
			{
				Indices.Add(0);
				Indices.Add(static_cast<SlateIndex>(k));
				Indices.Add(static_cast<SlateIndex>(k + 1));
			}
		}
		FSlateDrawElement::MakeCustomVerts(Out, Layer, Handle, Verts, Indices, nullptr, 0, 0);

		// Percent label inside slices big enough to hold it.
		if (Intro >= 1.f && V / Total >= 0.06f)
		{
			const FString Pct = FString::Printf(TEXT("%.0f%%"), 100.f * V / Total);
			const FVector2f S = Measure(Pct, LabelFont);
			const float Mid = Angle + Span * 0.5f;
			const FVector2f C = L.PieCenter + FVector2f(FMath::Cos(Mid), FMath::Sin(Mid)) * Radius * 0.62f;
			DrawText(Geo, Out, Layer + 1, C - S * 0.5f, Pct, LabelFont, FLinearColor::White);
		}
		Angle += Span;
	}
}

void SSlateLiteChart::PaintLegend(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	const bool bPie = ChartType == ESlateLiteChartType::Pie;
	const int32 Count = bPie ? Categories.Num() : Series.Num();
	float X = SlateLite::Pad;
	for (int32 i = 0; i < Count; ++i)
	{
		const FString Name = bPie ? Categories[i] : Series[i].Name;
		const FVector2f S = Measure(Name, LabelFont);
		if (X + 14.f + S.X > L.Size.X - SlateLite::Pad) { break; }
		const FLinearColor Color = bPie ? PaletteColor(i) : SeriesColor(i);
		DrawBox(Geo, Out, Layer, FVector2f(X, L.LegendY + (S.Y - 8.f) * 0.5f), FVector2f(8.f, 8.f), Color);
		DrawText(Geo, Out, Layer, FVector2f(X + 12.f, L.LegendY), Name, LabelFont, SlateLite::InkDim);
		X += 12.f + S.X + 14.f;
	}
}

void SSlateLiteChart::PaintTooltip(const FLayout& L, const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer) const
{
	if (HoverIndex == INDEX_NONE || !Categories.IsValidIndex(HoverIndex)) { return; }

	TArray<TPair<FString, FLinearColor>> Rows;
	if (ChartType == ESlateLiteChartType::Pie)
	{
		const float V = Series[0].Values.IsValidIndex(HoverIndex) ? Series[0].Values[HoverIndex] : NAN;
		Rows.Emplace(FMath::IsFinite(V) ? FText::AsNumber(V).ToString() : TEXT("-"), PaletteColor(HoverIndex));
	}
	else
	{
		for (int32 s = 0; s < Series.Num(); ++s)
		{
			const float V = Series[s].Values.IsValidIndex(HoverIndex) ? Series[s].Values[HoverIndex] : NAN;
			Rows.Emplace(Series[s].Name + TEXT("   ") + (FMath::IsFinite(V) ? FText::AsNumber(V).ToString() : TEXT("-")), SeriesColor(s));
		}
	}

	const float LineH = Measure(TEXT("Ag"), LabelFont).Y + 3.f;
	float W = Measure(Categories[HoverIndex], LabelFont).X;
	for (const TPair<FString, FLinearColor>& R : Rows) { W = FMath::Max(W, Measure(R.Key, LabelFont).X + 14.f); }
	const FVector2f Box(W + 20.f, LineH * (Rows.Num() + 1) + 14.f);

	FVector2f Pos = HoverPos + FVector2f(14.f, 14.f);
	if (Pos.X + Box.X > L.Size.X) { Pos.X = HoverPos.X - Box.X - 14.f; }
	if (Pos.Y + Box.Y > L.Size.Y) { Pos.Y = HoverPos.Y - Box.Y - 14.f; }
	Pos.X = FMath::Max(Pos.X, 0.f);
	Pos.Y = FMath::Max(Pos.Y, 0.f);

	DrawBox(Geo, Out, Layer, Pos, Box, SlateLite::Tip);
	DrawText(Geo, Out, Layer + 1, Pos + FVector2f(10.f, 7.f), Categories[HoverIndex], LabelFont, SlateLite::Ink);
	for (int32 r = 0; r < Rows.Num(); ++r)
	{
		const float Y = Pos.Y + 7.f + LineH * (r + 1);
		DrawBox(Geo, Out, Layer + 1, FVector2f(Pos.X + 10.f, Y + (LineH - 3.f - 8.f) * 0.5f), FVector2f(8.f, 8.f), Rows[r].Value);
		DrawText(Geo, Out, Layer + 1, FVector2f(Pos.X + 24.f, Y), Rows[r].Key, LabelFont, SlateLite::Ink);
	}
}

int32 SSlateLiteChart::HitTest(const FVector2f& Local, const FVector2f& Size) const
{
	if (Categories.Num() == 0 || Series.Num() == 0) { return INDEX_NONE; }
	const FLayout L = MakeLayout(Size);

	if (ChartType == ESlateLiteChartType::Pie)
	{
		const FVector2f D = Local - L.PieCenter;
		if (D.Size() > L.PieRadius) { return INDEX_NONE; }
		float A = FMath::Atan2(D.Y, D.X) + 0.5f * PI; // 0 at 12 o'clock, clockwise
		if (A < 0.f) { A += 2.f * PI; }
		const TArray<float>& Values = Series[0].Values;
		float Total = 0.f;
		for (int32 i = 0; i < Categories.Num() && i < Values.Num(); ++i) { if (FMath::IsFinite(Values[i]) && Values[i] > 0.f) { Total += Values[i]; } }
		if (Total <= 0.f) { return INDEX_NONE; }
		float Acc = 0.f;
		for (int32 i = 0; i < Categories.Num() && i < Values.Num(); ++i)
		{
			if (!FMath::IsFinite(Values[i]) || Values[i] <= 0.f) { continue; }
			Acc += 2.f * PI * Values[i] / Total;
			if (A <= Acc) { return i; }
		}
		return INDEX_NONE;
	}

	if (Local.X < L.Plot.Left || Local.X > L.Plot.Right || Local.Y < L.Plot.Top || Local.Y > L.Plot.Bottom) { return INDEX_NONE; }
	const float Band = (L.Plot.Right - L.Plot.Left) / Categories.Num();
	return FMath::Clamp(FMath::FloorToInt((Local.X - L.Plot.Left) / Band), 0, Categories.Num() - 1);
}

FReply SSlateLiteChart::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	HoverPos = FVector2f(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()));
	HoverIndex = HitTest(HoverPos, FVector2f(MyGeometry.GetLocalSize()));
	Invalidate(EInvalidateWidgetReason::Paint);
	return FReply::Unhandled();
}

void SSlateLiteChart::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SLeafWidget::OnMouseLeave(MouseEvent);
	HoverIndex = INDEX_NONE;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSlateLiteChart::DrawBox(const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer, const FVector2f& Pos, const FVector2f& Size, const FLinearColor& Color) const
{
	FSlateDrawElement::MakeBox(Out, Layer, Geo.ToPaintGeometry(Size, FSlateLayoutTransform(Pos)), WhiteBrush, ESlateDrawEffect::None, Color);
}

void SSlateLiteChart::DrawText(const FGeometry& Geo, FSlateWindowElementList& Out, int32 Layer, const FVector2f& Pos, const FString& Text, const FSlateFontInfo& Font, const FLinearColor& Color) const
{
	FSlateDrawElement::MakeText(Out, Layer, Geo.ToPaintGeometry(Measure(Text, Font), FSlateLayoutTransform(Pos)), Text, Font, ESlateDrawEffect::None, Color);
}

FVector2f SSlateLiteChart::Measure(const FString& Text, const FSlateFontInfo& Font) const
{
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	return FVector2f(FontMeasure->Measure(Text, Font));
}
