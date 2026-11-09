---
title: Feeding data
---

# Feeding data

## Blueprint

Build the struct (`Make Slate Category Data`, `Make Single Series Data`, `Make Pie Data`, or the struct's own Make node) and call **Set Data** on the chart. The chart animates from what is on screen to the new values.

Live feeds: **Push Point** on a line chart appends a value and scrolls the oldest one off. Gauges and stat tiles have **Set Value**, heatmaps **Set Cell**, calendar heatmaps **Set Day**.

## JSON

**Set Data From Json** takes the same data as text. Field names match the struct properties and are case-insensitive. An optional `title` and `subtitle` update the header, and colors may be hex strings.

Category data (bar, line, area, combo, heatmap, radial bar, bump, wind rose, lollipop):

```json
{
  "title": "Permits issued",
  "subtitle": "By month",
  "categories": ["Jan", "Feb", "Mar"],
  "series": [
    { "name": "Residential", "values": [120, 140, 98] },
    { "name": "Commercial",  "values": [80, 72, 101], "color": "#22D3A6" }
  ]
}
```

Pie data (pie, doughnut, funnel, waffle, packed bubbles):

```json
{ "items": [ { "label": "Solar", "value": 312 }, { "label": "Wind", "value": 248 } ] }
```

Sankey data:

```json
{ "links": [ { "source": "Solar", "target": "Grid", "value": 42 }, { "source": "Grid", "target": "Homes", "value": 48 } ] }
```

## Missing values

NaN, infinity, and values past the end of a shorter series count as missing. Lines and areas break into a gap, bars are left out, the tooltip shows "–", and the axis ignores them.

## Mark lines, bands and forecasts

Bar, line, area and combo charts take:

- `MarkLines`: a value, or the average, median, min or max of a series, drawn as a labelled line.
- `MarkBands`: a shaded value range, for targets or thresholds.
- `bLabelExtremes`: a pin on each series' highest and lowest point.
- A series with `LowValues` and `HighValues` gets a soft range band around its line.
- `DashedFrom` draws a series dashed from that category on, for forecasts.
