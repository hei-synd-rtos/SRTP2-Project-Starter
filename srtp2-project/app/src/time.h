#pragma once

/**
 * @brief Initializes the Time module, configures GPIO pins, and synchronizes with the SRTP2 network.
 *
 * This function performs the following tasks:
 * - Checks if the GPIO pins are ready for usage. If not, logs an error and exits early.
 * - Configures two GPIO pins (ld1 and ld2) as active-output pins.
 * - Sets the initial state of the GPIO pins to LOW.
 * - Sends a synchronization request to the SRTP2 network stack using the provided syncIndication callback.
 * - Logs an error if the synchronization request fails.
 * - Logs a message indicating that the application has started successfully.
 */
void Time_init();
