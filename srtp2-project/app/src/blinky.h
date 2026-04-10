#pragma once

/**
 * @brief Initializes the Blinky application.
 *
 * This function configures the GPIO for the LED, starts the Blinky thread, and logs the initialization status.
 *
 * It first checks if the GPIO device tree specification is ready. If the GPIO is not ready, an error is logged, and the function terminates without further
 * execution.
 *
 * After confirming the GPIO is ready, it configures the GPIO pin as an active output. Then, it creates a thread to handle the Blinky behavior using the
 * specified thread name and priority. If the thread creation fails, an error is logged.
 *
 * Upon successful initialization, a log message indicates that the application has started.
 */
void Blinky_init();
