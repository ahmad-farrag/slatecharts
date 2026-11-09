---
title: Chart widgets
---

# Chart widgets

All widgets are in the UMG Palette under **Slate Charts**.

| Widget | Data property | Notable options |
|---|---|---|
| Slate Bar Chart | `FSlateCategoryData` | stacked, horizontal, value labels, corner radius |
| Slate Line Chart | `FSlateCategoryData` | smooth, points, fill, stacked, `PushPoint` for live feeds |
| Slate Area Chart | `FSlateCategoryData` | line chart with gradient fill on |
| Slate Combo (Line + Bar) Chart | `FSlateCategoryData` | per-series `Type`, right-hand axis via `bSecondaryAxis` |
| Slate Pie Chart | `FSlatePieData` | inner radius, rose mode, percent labels, slice gap |
| Slate Doughnut Chart | `FSlatePieData` | total in the hole, hovered slice replaces it |
| Slate Radar Chart | `FSlateRadarData` | rings, circular grid |
| Slate Funnel Chart | `FSlatePieData` | sort, conversion column |
| Slate Sankey Chart | `FSlateSankeyData` | nodes optional; columns laid out from links |
| Slate Scatter / Bubble Chart | `FSlateScatterData` | bubble size from `Size` |
| Slate Gauge Chart | `FSlateGaugeData` | colored bands, sweep, `SetValue` |
| Slate Radial Bar Chart | `FSlateCategoryData` | max value, sweep |
| Slate Heatmap Chart | `FSlateCategoryData` (categories = columns, series = rows) | sequential or diverging scale, fixed range, cell values, `SetCell` |
| Slate Calendar Heatmap | `FSlateCalendarData` (start date + one value per day) | week-start day, colors, `SetDay` |
| Slate Boxplot Chart | `FSlateBoxplotData` (raw samples per category) | quartiles, 1.5×IQR whiskers and outliers computed for you |
| Slate Treemap Chart | `FSlateTreeData` (flat items with `Parent` ids) | click a group to open it, breadcrumbs back, `DrillTo` |
| Slate Sunburst Chart | `FSlateTreeData` | rings per level, click to drill, center to go back, `DrillTo` |
| Slate Graph (Network) Chart | `FSlateGraphData` (nodes, links, group names) | force layout that settles live, neighbourhood highlight, group legend |
| Slate Stat Tile | `FSlateStatData` (value, comparison, trend) | count-up number, good/bad delta pill (`bHigherIsBetter`), sparkline, `SetValue` |
| Slate Bullet Chart | `FSlateBulletData` (value, target, range thresholds per row) | qualitative bands, target tick turns green when met |
| Slate Waterfall Chart | `FSlateWaterfallData` (start, steps, `bIsTotal` subtotals) | increase / decrease / total colors, connectors, value labels |
| Slate Histogram | `FSlateHistogramData` (raw samples) | automatic round-number bins or `BinCount`, mean and median markers |
| Slate Table | `FSlateTableData` (columns, rows with values and trend) | in-cell bar column, trend sparklines, click a header to sort, wheel scroll |
| Slate Timeline | `FSlateTimelineData` (rows of time spans) | Gantt with `Progress`, or state timeline with `State` names, "now" marker, calendar-aware axis |
| Slate Waffle Chart | `FSlatePieData` | grid size, largest-remainder squares, share list |
| Slate Bump Chart | `FSlateCategoryData` | ranks computed per category (or `bValuesAreRanks`), rank swaps animate |
| Slate Packed Bubbles | `FSlatePieData` | area-true circles, tight packing, labels when they fit |
| Slate Wind Rose | `FSlateCategoryData` (categories = directions from N clockwise, series = speed bins) | stacked petals, rings |
| Slate Lollipop Chart | `FSlateCategoryData` | one series = lollipops, two = dumbbells with change in the tooltip, sorted rows |

## Options every chart shares

- **Header** (`FSlateChartHeader`): title, subtitle, legend position, legend click-to-toggle, tooltip on or off.
- **Value Format** (`FSlateValueFormat`): plain, compact (12.3K) or percent, decimals, prefix, suffix.
- **Theme**: Project Default, Midnight, Graphite, Light, Ocean, Sunset, or Custom. See [Look and project settings](settings).
- **Font** (`FSlateChartFont`): typeface for all of the chart's text.
- **Transparent Background** and **Background Contrast**: drop the chart card so the chart sits on your scene.
- **OnItemClicked**(SeriesIndex, ItemIndex, Label, Value).
- **Set Data** animates to new data. **Play Intro** replays the entry animation.
- **Set Data From Json** and **Preview Pointer At** (0..1 position).

## Extras on bar, line, area and combo charts

- `MarkLines`: a fixed value, or the average, median, min or max of a series.
- `MarkBands`: a shaded value range.
- `bLabelExtremes`: pins on each series' highest and lowest point.
- `LowValues` / `HighValues` on a series: a soft range band around its line.
- `DashedFrom` on a series: the line turns dashed from that category on, for forecasts.
- `bDataZoom`: a range slider under the chart. See [Interaction](interaction#data-zoom).
