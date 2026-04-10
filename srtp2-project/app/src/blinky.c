#include "blinky.h"
#include <zephyr/drivers/gpio.h>
#include <cmsis_os2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(blinky, LOG_LEVEL_INF);

static const struct gpio_dt_spec ld0_ = GPIO_DT_SPEC_GET(DT_ALIAS(extled0), gpios);

static void Blinky_threadFunction_(void*);

void Blinky_init() {
    // Check if the GPIO is ready.
    if (!gpio_is_ready_dt(&ld0_)) {
        LOG_ERR("GPIO not ready");
        return;
    }

    // Configure the GPIO.
    gpio_pin_configure_dt(&ld0_, GPIO_OUTPUT_ACTIVE);

    // Start the thread.
    osThreadAttr_t blinkyThreadAttributes = {
        .name = "Blinky",
        .priority = osPriorityBelowNormal
    };
    if (osThreadNew(Blinky_threadFunction_, &blinkyThreadAttributes, NULL) <= 0) {
        LOG_ERR("failed to create thread");
    }

    LOG_INF("application started");
}

static void Blinky_threadFunction_(void*) {
    // Get the tick frequency.
    uint32_t freq = osKernelGetTickFreq();

    // Toggle the LED with a frequency of 2 Hz.
    while (true) {
        gpio_pin_toggle_dt(&ld0_);
        osDelay(250 * freq / 1000);
    }
}
