#pragma once

/**
 * @brief Initializes the Nunchuk application module.
 *
 * This function performs the following operations to initialize the Nunchuk module:
 * - Sets the default state of the joystick position to the center (127, 127).
 * - Resets the acceleration values to zero for all axes.
 * - Registers a synchronization request callback using the SRTP2_syncRequest() function.
 * - Publishes sampled values (SV) groups for joystick position (0) and acceleration (1) using the SRTP2_svPublishRequest() function.
 * - Sets the `ready` flag to true, indicating that the application has initialized successfully.
 *
 * In case of any failure during the initialization process (e.g., failing to register a sync request or publish SV groups), an error is logged, and
 * the function returns without completing initialization.
 */
void Nunchuk_init();
