/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Minimal pointing.h for the Keychron ZMK fork.
 * Provides MB1-MB5 (mouse buttons) and SCRL_* (scroll directions)
 * for use with &mkp and &msc behaviors.
 */
#pragma once

/* Mouse buttons: standard USB HID bitmask. */
#define MB1 0x01
#define MB2 0x02
#define MB3 0x04
#define MB4 0x08
#define MB5 0x10

#define LCLK MB1
#define RCLK MB2
#define MCLK MB3

/* Scroll directions. Encoded so the C side can switch on them. */
#define SCRL_UP    1
#define SCRL_DOWN  2
#define SCRL_LEFT  3
#define SCRL_RIGHT 4
