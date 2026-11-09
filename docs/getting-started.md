---
title: Getting started
---

# Getting started

## Install

**From Fab:** add SlateCharts to your library, then install it to your engine from the Fab Library in the Epic Games Launcher. Open your project, go to **Edit > Plugins**, search for SlateCharts, enable it and restart the editor.

**Into a project:** copy the `SlateCharts` folder into your project's `Plugins/` folder (create it if needed), then reopen the project. A C++ project rebuilds the plugin; a Blueprint-only project needs the engine-installed version from Fab.

SlateCharts targets Unreal Engine 5.8.

## Add a chart

1. Open or create a Widget Blueprint.
2. In the Palette, open **Slate Charts** and drag a chart (for example **Slate Bar Chart**) onto the canvas.
3. Size it. Every chart comes with sample data, so you see it straight away in the designer.
4. In the Details panel, set the title under **Header** and the data under **Data**.

## Give it data from Blueprint

Build the data struct and call **Set Data** on the chart:

1. Drag the chart variable into the graph.
2. Use **Make Slate Category Data** (bar, line, area, combo and similar charts), **Make Pie Data** (pie, doughnut, funnel, waffle, packed bubbles) or the Make node of the chart's data struct.
3. Call **Set Data**. The chart animates from what it shows now to the new values.

To load the same data from text, call **Set Data From Json** with a JSON string. See [Feeding data](data).

## Try the showcase

The plugin includes a showcase level: **Content Browser > Plugins > SlateCharts Content > Showcase > L_SlateChartsShowcase**. Open it and press Play. The buttons along the top switch the theme, shuffle the data, replay the intro, and toggle the chart card, contrast, backdrop and font. The **Stress tests** tab shows how the charts handle awkward data and sizes.

## Shipping builds

The plugin has three modules:

| Module | What it is | In Shipping builds |
|---|---|---|
| `SlateCharts` | The chart widgets, Slate plots, settings and Blueprint library | Yes |
| `SlateChartsShowcase` | Showcase level game mode, gallery, stress tests, benchmark | No |
| `SlateChartsEditor` | Editor commandlet and automation tests | No |

Game code should depend only on `SlateCharts`. Leave the showcase level out of Shipping cooks.
