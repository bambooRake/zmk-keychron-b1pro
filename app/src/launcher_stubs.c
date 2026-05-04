/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 * Stub implementations for launcher symbols. Provided so the firmware
 * can link with CONFIG_ZMK_LAUNCHER=n while still letting endpoints.c
 * and behavior_macro.c call into these entry points.
 *
 * When CONFIG_ZMK_LAUNCHER=y the real implementations live in
 * app/src/launcher/ and these stubs are skipped.
 */

#include <stdint.h>
#include <stdbool.h>

/* autoconf.h is force-included by the Zephyr build (-imacros), so
 * CONFIG_* macros are visible without an explicit include. */

#if !defined(CONFIG_ZMK_LAUNCHER) || (CONFIG_ZMK_LAUNCHER == 0)

void mousekey_send(void) {}

void dynamic_keymap_macro_send(uint8_t id) { (void)id; }

#endif

/* user_set_debounce(), all_keys_up(), matrix_get_row(): runtime helpers
 * exposed by Keychron's patched kscan_gpio_matrix.c. We've reverted
 * the kscan driver to upstream (the Keychron v1.0.5 changes broke
 * alphabet keys), so callers in activity.c / launcher.c get no-op
 * stubs. activity sleep behavior loses "all keys up" optimization
 * but functionally still works. */
__attribute__((weak)) void user_set_debounce(uint32_t scan_period_ms,
                                              uint32_t debounce_press_ms,
                                              uint32_t debounce_release_ms) {
    (void)scan_period_ms;
    (void)debounce_press_ms;
    (void)debounce_release_ms;
}

__attribute__((weak)) bool all_keys_up(void) {
    /* Be conservative: claim keys are up so callers (e.g. sleep checks)
     * don't get stuck in an "always pressed" state. */
    return true;
}

__attribute__((weak)) uint32_t matrix_get_row(uint8_t row) {
    (void)row;
    return 0;
}
