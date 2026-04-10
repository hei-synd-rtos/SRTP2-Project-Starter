#include "time.h"
#include <srtp2.h>
#include <zephyr/drivers/gpio.h>
#include <cmsis_os2.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include <zephyr/logging/log_ctrl.h>

LOG_MODULE_REGISTER(time, LOG_LEVEL_INF);

static const struct gpio_dt_spec ld1_ = GPIO_DT_SPEC_GET(DT_ALIAS(extled1), gpios);
static const struct gpio_dt_spec ld2_ = GPIO_DT_SPEC_GET(DT_ALIAS(extled2), gpios);

static bool ledState_ = false;

static void Time_syncIndication_(SRTP2_NetworkID_t, SRTP2_NetworkTime_t, SRTP2_CycleInterval_t, SRTP2_CallbackContext_t);
static log_timestamp_t Time_getSystemTime_();

void Time_init() {
    // Check if the GPIO is ready.
    if (!gpio_is_ready_dt(&ld1_)) {
        LOG_ERR("GPIO not ready");
        return;
    }

    // Configure the GPIO.
    gpio_pin_configure_dt(&ld1_, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&ld2_, GPIO_OUTPUT_ACTIVE);

    gpio_pin_set_dt(&ld1_, 0);
    gpio_pin_set_dt(&ld2_, 0);

    // Register sync service at SRTP2 stack.
    SRTP2_Status_t status = SRTP2_syncRequest(Time_syncIndication_, NULL);
    if (status != SRTP2_OK) {
        LOG_ERR("sync request failed");
    }

    // Register custom log time format function.
    log_set_timestamp_func(Time_getSystemTime_, 1000);

    LOG_INF("application started.");
}

static void Time_syncIndication_(SRTP2_NetworkID_t networkID, SRTP2_NetworkTime_t networkTime, SRTP2_CycleInterval_t cycleInterval,
    SRTP2_CallbackContext_t context) {

    // Check drift and update system clock if required.
    struct timespec ts;
    if (sys_clock_gettime(SYS_CLOCK_REALTIME, &ts) == 0) {
        SRTP2_NetworkTime_t currentTime = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
        int64_t delta = ((int64_t)currentTime - (int64_t)networkTime);
        if (delta > 10 || delta < -10) {
            if (delta > -1000000000) {
                int32_t deltaPrint = delta;
                LOG_WRN("network time drift is %ims, updating system time", deltaPrint);
            }
            ts.tv_sec = networkTime / 1000;
            ts.tv_nsec = (networkTime % 1000) * 1000000;
            if (sys_clock_settime(SYS_CLOCK_REALTIME, &ts) != 0) {
                LOG_ERR("failed to update system time");
            }
        }
    } else {
        LOG_ERR("failed to get system time");
    }

    // Toggle LD1 each sync event.
    gpio_pin_toggle_dt(&ld1_);

    // Turn on LD2 each 10 seconds for 1 second.
    bool newLedState = (networkTime / 1000) % 10 == 0;
    if (newLedState != ledState_) {
        ledState_ = newLedState;
        gpio_pin_set_dt(&ld2_, ledState_);
    }
}

static log_timestamp_t Time_getSystemTime_() {
    struct timespec ts;

    // Fetch the time using the native API
    if (sys_clock_gettime(SYS_CLOCK_REALTIME, &ts) == 0) {
        // Convert to milliseconds
        return ((uint64_t)ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    }
    return k_uptime_get();
}
