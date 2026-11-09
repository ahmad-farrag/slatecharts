---
title: SlateCharts Lite
---

# SlateCharts Lite

SlateCharts Lite is a free, minimal chart widget for UMG: one widget that draws a bar, line or pie chart in Slate, with a hover readout and a short grow-in animation. It is MIT-licensed, so you can read it, change it and ship it.

Source: [`Lite/SlateChartsLite`](https://github.com/ahmad-farrag/slatecharts/tree/main/Lite/SlateChartsLite)

## Install

1. Copy `Lite/SlateChartsLite` into your project's `Plugins/` folder.
2. Open the project (a C++ project compiles the plugin; for a Blueprint-only project, add any C++ class once so the editor can build it).
3. **Edit > Plugins**, make sure **SlateCharts Lite** is enabled.

Built and tested on Unreal Engine 5.8.

## Use it

1. In a Widget Blueprint, drag **Slate Lite Chart** from the Palette (category **SlateCharts Lite**).
2. Set **Chart Type** (Bar, Line or Pie), **Title**, **Categories** and **Series** in the Details panel. It starts with sample data.
3. At runtime, call **Set Data** (categories + series), **Set Chart Type** or **Set Title**.

A series has a `Name`, a `Color` (leave alpha at 0 to use the built-in palette) and one value per category. A pie chart uses the first series: one slice per category.

## C++

```cpp
#include "SSlateLiteChart.h"

TSharedRef<SSlateLiteChart> Chart = SNew(SSlateLiteChart);

FSlateLiteSeries Sales;
Sales.Name = TEXT("Sales");
Sales.Values = { 120.f, 140.f, 98.f };

Chart->SetTitle(FText::FromString(TEXT("Q1")));
Chart->SetData({ TEXT("Jan"), TEXT("Feb"), TEXT("Mar") }, { Sales });
```

Add `SlateChartsLite` to your module's dependencies.

## Need more?

The full [SlateCharts](./) plugin on Fab adds about 30 chart types, themes, project-wide defaults, JSON input, data zoom, drill-down, click-to-pin, touch, keyboard and gamepad support, transparent mode, custom fonts, frame caching and a showcase level.
