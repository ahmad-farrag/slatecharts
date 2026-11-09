---
title: Interaction
---

# Interaction

Every chart shares one focus model, so the behaviour is the same everywhere.

## Mouse

- **Hover** a mark to focus it. The other marks fade back, and focus changes crossfade (`Header.FocusFadeTime`, 0.18 s by default) so fast pointer movement never flickers. The tooltip fades in and follows the pointer.
- **Two levels of focus.** Hovering a category's empty space focuses the whole group (every series in the tooltip). Hovering the exact bar, point, radar vertex or bubble focuses just that mark: a value pill appears and its tooltip row is highlighted.
- **Labels are targets too.** Radar indicator names and radial-bar labels focus their spoke or ring.
- **Legend hover** (`Header.bLegendHoverFocus`) brings that series or slice forward everywhere. **Legend click** hides or shows it.
- **Click to pin** (`Header.bClickToPin`): the highlight and tooltip stay until you click the same mark again, click empty space, press Esc, or focus moves elsewhere. Blueprint: `PinAt`, `ClearPin`, `IsPinned`.
- **Smooth rescaling.** Hiding a series tweens axis ranges, bar widths, funnel stages and rose angles instead of snapping.

## Data zoom

Set `bDataZoom` on a bar, line, area or combo chart for a range slider under it. Drag the window or its handles, or use the mouse wheel over the chart to zoom around the pointer. Blueprint: `SetZoomRange(Start, End)` (0..1) and `ResetZoom`.

## Drill-down

Treemaps and sunbursts open a group when you click it. Breadcrumbs (treemap) or the centre (sunburst) take you back. Blueprint: `DrillTo`.

## Touch

Touch screens get their own gestures; there is nothing to switch on.

- **Tap** a mark to show its tooltip. Tap it again, tap empty space, or touch anything else to put it away.
- **Slide sideways, or press and hold** (0.35 s), to scrub. The tooltip is drawn above your finger so your hand doesn't cover it.
- **Slide up or down** to scroll whatever the chart sits in.
- **Pinch** a chart with `bDataZoom` to zoom around your fingers, and move them together to pan.

To try touch with a mouse, turn on **Project Settings > Input > Use Mouse For Touch** (one finger only, so no pinch).

## Keyboard and gamepad

Every chart takes keyboard focus, so **Tab** / **Shift+Tab** stop on it like on a button.

| Keyboard | Gamepad | Does |
|---|---|---|
| ← → | D-pad, left stick | Step through categories, slices or points |
| ↑ ↓ | D-pad, left stick | Step through the series in the current item |
| Enter, Space | A | Drill, sort, or pin the reading. Fires `OnItemClicked` |
| Esc | B | Unpin; press again to put the reading away |
| + / = | Right shoulder | Zoom in (charts with `bDataZoom`) |
| - | Left shoulder | Zoom out |
| Home | Y | Show the whole range |

An arrow with nowhere to go hands focus to the next widget in that direction, so a gamepad can leave the chart. A focus ring shows when focus came from the keyboard or a gamepad.

If the project's analog cursor is running, it turns the gamepad into a mouse and the D-pad navigation above doesn't apply.

## Click events

`OnItemClicked(SeriesIndex, ItemIndex, Label, Value)` fires for clicks, taps and Enter / A on a mark.
