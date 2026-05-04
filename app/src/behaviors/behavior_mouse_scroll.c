/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Minimal mouse-scroll behavior for the Keychron ZMK fork.
 * Sends a single-tick scroll report on press; press-and-hold sends
 * one report per press event (no auto-repeat). Direction comes from
 * the SCRL_* constants in dt-bindings/zmk/pointing.h.
 */

#define DT_DRV_COMPAT zmk_behavior_mouse_scroll

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <dt-bindings/zmk/pointing.h>

#include "../launcher/mousekey.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int behavior_mouse_scroll_init(const struct device *dev) { return 0; }

static int on_msc_pressed(struct zmk_behavior_binding *binding,
                          struct zmk_behavior_binding_event event) {
    report_mouse_t report = { ZMK_HID_REPORT_ID_MOUSE, 0, 0, 0, 0, 0 };

    switch (binding->param1) {
    case SCRL_UP:
        report.v = 1;
        break;
    case SCRL_DOWN:
        report.v = -1;
        break;
    case SCRL_LEFT:
        report.h = -1;
        break;
    case SCRL_RIGHT:
        report.h = 1;
        break;
    default:
        LOG_WRN("unknown msc direction: %d", binding->param1);
        return -EINVAL;
    }

    LOG_DBG("msc dir=%d v=%d h=%d", binding->param1, report.v, report.h);
    int err = host_mouse_send(&report);

    /* Send a stop report so the scroll does not repeat on the host. */
    report.v = 0;
    report.h = 0;
    host_mouse_send(&report);

    return err;
}

static int on_msc_released(struct zmk_behavior_binding *binding,
                           struct zmk_behavior_binding_event event) {
    return 0;
}

static const struct behavior_driver_api behavior_mouse_scroll_driver_api = {
    .binding_pressed = on_msc_pressed,
    .binding_released = on_msc_released,
};

#define MSC_INST(n)                                                                                \
    DEVICE_DT_INST_DEFINE(n, behavior_mouse_scroll_init, NULL, NULL, NULL, APPLICATION,            \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                     \
                          &behavior_mouse_scroll_driver_api);

DT_INST_FOREACH_STATUS_OKAY(MSC_INST)
