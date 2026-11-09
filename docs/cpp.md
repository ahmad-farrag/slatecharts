---
title: C++ and raw Slate
---

# C++ and raw Slate

## Dependencies

Add `SlateCharts` to your module's `PublicDependencyModuleNames`. Don't depend on `SlateChartsShowcase` or `SlateChartsEditor`; they are not built for Shipping.

## Using a chart widget from C++

The UMG widgets (`USlateBarChart`, `USlateLineChart` and the rest) are normal `UWidget`s. Set their data struct and call `SetData`, or `SetDataFromJson` with a string.

## Using the plots without UMG

`SSlateChart` hosts any `ISlatePlot`. The plot classes in `Slate/SlatePlots.h` work without UMG, so you can put a chart straight into a Slate layout or an editor tool.

## Writing a new chart type

A new chart type is a new `ISlatePlot`. Implement:

- `Paint`
- `HitTest`
- `GetLegend`
- `Retarget` (animate to new data)
- `ResetToEmpty`

Use `FSlatePainter` for anti-aliased arcs, polygons, ribbons, rounded boxes and text.

Plots that need their own input override the optional hooks: `OnWheel`, `BeginDrag` / `OnDrag` / `EndDrag` (the host captures the mouse), `OnClick` (return `HandledRefresh` to re-animate after a drill), `IsDragHandle` (grab cursor) and `Tick` (per-frame motion such as zoom easing or a settling layout). Keyboard and gamepad hooks: `OnZoomKey`, `OnResetView`, `OnStepView`.

## Frame caching

`SSlateChart` records each painted frame and replays it until something changes, so idle charts cost almost nothing. A plot is repainted after the host's own calls (`Refresh`, `PlotViewChanged`, input the plot answers with `Handled` or `HandledRefresh`) and on every frame its `Tick` returns true. Any other change to what a plot draws must go through one of those, or the chart keeps showing the old frame.

The console variable `SlateCharts.PaintCache 0` turns caching off (every chart lays out and tessellates on every paint), for comparing performance. It is compiled out of Shipping, where caching is always on.

## Automated tests

The editor module has automation tests for touch, keyboard and paint caching. Run them from **Session Frontend > Automation**, or headless:

```
UnrealEditor-Cmd.exe <Project>.uproject -ExecCmds="Automation RunTests SlateCharts; Quit" -unattended -nullrhi
```
