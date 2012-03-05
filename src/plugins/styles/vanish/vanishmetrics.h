/****************************************************************************
 * This file is part of Vanish.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vanish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation.
 *
 * Vanish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vanish.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VANISHMETRICS_H
#define VANISHMETRICS_H

enum Metrics {
    // Glow for borders
    GlowWidthMetric = 2,

    // Check boxes
    CheckBoxSizeMetric = 21,
    CheckBoxTextSpaceMetric = 4,

    // Combo boxes
    ComboBoxFrameWidthMetric = 2,
    ComboBoxButtonWidthMetric = 19,
    ComboBoxButtonMarginMetric = 2,
    ComboBoxButtonMarginLeftMetric = 0,
    ComboBoxButtonMarginRightMetric = 4,
    ComboBoxButtonMarginTopMetric = 2,
    ComboBoxButtonMarginBottomMetric = 1,
    ComboBoxContentsMarginMetric = 0,
    ComboBoxContentsMarginLeftMetric = 2,
    ComboBoxContentsMarginRightMetric = 0,
    ComboBoxContentsMarginTopMetric = 0,
    ComboBoxContentsMarginBottomMetric = 0,

    // Dock widgets
    DockWidgetFrameWidthMetric = 0,
    DockWidgetSeparatorExtentMetric = 3,
    DockWidgetTitleMarginMetric = 3,

    // Generic frames
    DefaultFrameWidthMetric = 3,

    // Group boxes
    GroupBoxFrameWidthMetric = 3,

    // Headers
    HeaderTextToIconSpaceMetric = 3,
    HeaderContentsMarginMetric = 3,

    // Line edit
    LineEditFrameWidthMetric = 3,

    // Menu items
    MenuItemAccelSpaceMetric = 16,
    MenuItemArrowWidthMetric = 11,
    MenuItemArrowSpaceMetric = 3,
    MenuItemCheckWidthMetric = 16,
    MenuItemCheckSpaceMetric = 3,
    MenuItemIconWidthMetric = 12,
    MenuItemIconSpaceMetric = 3,
    MenuItemMarginMetric = 2,
    MenuItemMinHeightMetric = 20,

    // Menu bar item
    MenuBarItemMarginMetric = 3,
    MenuBarItemMarginLeftMetric = 5,
    MenuBarItemMarginRightMetric = 5,

    // Push buttons
    PushButtonContentsMarginMetric = 5,
    PushButtonContentsMarginLeftMetric = 8,
    PushButtonContentsMarginTopMetric = 1,
    PushButtonContentsMarginRightMetric = 8,
    PushButtonContentsMarginBottomMetric = 0,
    PushButtonMenuIndicatorSizeMetric = 8,
    PushButtonTextToIconSpaceMetric = 6,

    // Progress bars
    ProgressBarBusyIndicatorSizeMetric = 10,
    ProgressBarGrooveMarginMetric = 0,

    // Scroll bars
    ScrollBarExtentSizeMetric = 20,
    ScrollBarMinSliderHeightMetric = 21,

    // Slider groove height
    SliderGrooveWidthMetric = 7,

    // Spin boxes
    SpinBoxFrameWidthMetric = 3,
    SpinBoxButtonWidthMetric = 19,
    SpinBoxButtonMarginMetric = 0,
    SpinBoxButtonMarginLeftMetric = 2,
    SpinBoxButtonMarginRightMetric = 6,
    SpinBoxButtonMarginTopMetric = 4,
    SpinBoxButtonMarginBottomMetric = 2,

    // Splitter
    SplitterWidthMetric = 3,

    // Tabs
    TabBarBaseOverlapMetric = 7,
    TabBarBaseHeightMetric = 2,
    TabBarScrollButtonWidthMetric = 18,
    TabBarTabContentsMarginMetric = 4,
    TabBarTabContentsMarginLeftMetric = 5,
    TabBarTabContentsMarginRightMetric = 5,
    TabBarTabContentsMarginTopMetric = 2,
    TabBarTabContentsMarginBottomMetric = 4,
    TabBarTabOverlapMetric = 0,
    TabWidgetContentsMarginMetric = 4,

    // Tool buttons
    ToolButtonContentsMarginMetric = 4,
    ToolButtonInlineMenuIndicatorSizeMetric = 8,
    ToolButtonInlineMenuIndicatorXOffMetric = -11,
    ToolButtonInlineMenuIndicatorYOffMetric = -10,
    ToolButtonMenuIndicatorSizeMetric = 11,

    // Tree max expander size
    TreeMaxExpanderSizeMetric = 9
};

#endif // VANISHMETRICS_H
