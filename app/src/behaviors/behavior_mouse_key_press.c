/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Minimal mouse-button press/release behavior for the Keychron ZMK fork.
 * Builds on the existing host_mouse_send() infrastructure exposed by
 * app/src/launcher/mousekey.h. Maintains a single global button bitmask
 * so multiple buttons can be held concurrently across separate keys.
 */

#define DT_DRV_COMPAT zmk_behavior_mouse_key_press

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/hid.h>

#include "../launcher/mousekey.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static report_mouse_t mkp_report = { ZMK_HID_REPORT_ID_MOUSE, 0, 0, 0, 0, 0 };

static int behavior_mouse_key_press_init(const struct device *dev) { return 0; }

static int on_mkp_pressed(struct zmk_behavior_binding *binding,
                          struct zmk_behavior_binding_event event) {
    LOG_DBG("mkp pressed: 0x%02X", binding->param1);
    mkp_report.buttons |= (uint8_t)binding->param1;
    mkp_report.x = 0;
    mkp_report.y = 0;
    mkp_report.v = 0;
    mkp_report.h = 0;
    return host_mouse_send(&mkp_report);
}

static int on_mkp_released(struct zmk_behavior_binding *binding,
                           struct zmk_behavior_binding_event event) {
    LOG_DBG("mkp released: 0x%02X", binding->param1);
    mkp_report.buttons &= ~((uint8_t)binding->param1);
    mkp_report.x = 0;
    mkp_report.y = 0;
    mkp_report.v = 0;
    mkp_report.h = 0;
    return host_mouse_send(&mkp_report);
}

static const struct behavior_driver_api behavior_mouse_key_press_driver_api = {
    .binding_pressed = on_mkp_pressed,
    .binding_released = on_mkp_released,
};

#define MKP_INST(n)                                                                                \
    DEVICE_DT_INST_DEFINE(n, behavior_mouse_key_press_init, NULL, NULL, NULL, APPLICATION,         \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                     \
                          &behavior_mouse_key_press_driver_api);

DT_INST_FOREACH_STATUS_OKAY(MKP_INST)
