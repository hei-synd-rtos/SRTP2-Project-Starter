#include "nunchuk.h"
#include <srtp2.h>
#include <zephyr/input/input.h>
#include <zephyr/logging/log.h>
#include <input_nunchuk.h>

LOG_MODULE_REGISTER(nunchuk, LOG_LEVEL_INF);

#pragma pack(push, 1)
typedef struct {
    uint8_t x;
    uint8_t y;
} Nunchuk_JoystickPosition_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Nunchuk_Acceleration_t;
#pragma pack(pop)

static const struct device* nunchukDevice_ = DEVICE_DT_GET(DT_NODELABEL(nunchukng));

static bool ready_ = false;
static Nunchuk_JoystickPosition_t joystickPosition_;
static Nunchuk_Acceleration_t acceleration_;

enum {
    NUNCHUK_JOYSTICK_SV_GROUP_ = 0,
    NUNCHUK_ACCELERATION_SV_GROUP_ = 1,
    EV_BUTTON_PRESSED_ = 0,
    EV_BUTTON_RELEASED_ = 1,
    NUNCHUK_Z_BUTTON_ID_ = 0,
    NUNCHUK_C_BUTTON_ID_ = 1,
    BTN0_ID_ = 2,
    BTN1_ID_ = 3,
    BTN2_ID_ = 4,
    BTN3_ID_ = 5,
    INVALID_BUTTON_ID = 0xFF
};

static void Nunchuk_syncIndication_(SRTP2_NetworkID_t, SRTP2_NetworkTime_t, SRTP2_CycleInterval_t, SRTP2_CallbackContext_t);

void Nunchuk_init() {
    // Initialize
    joystickPosition_.x = 127;
    joystickPosition_.y = 127;
    acceleration_.x = 0;
    acceleration_.y = 0;
    acceleration_.z = 0;

    // Register for synchronization.
    if (SRTP2_syncRequest(Nunchuk_syncIndication_, NULL) != SRTP2_OK) {
        LOG_ERR("failed to register sync request");
        return;
    }

    // Register to publish sampled values for Nunchuk joystick position.
    if (SRTP2_svPublishRequest(NUNCHUK_JOYSTICK_SV_GROUP_, sizeof(Nunchuk_JoystickPosition_t)) != SRTP2_OK) {
        LOG_ERR("failed to register SV group for nunchuk joystick position");
        return;
    }

    // Register to publish sampled values for Nunchuk acceleration.
    if (SRTP2_svPublishRequest(NUNCHUK_ACCELERATION_SV_GROUP_, sizeof(Nunchuk_Acceleration_t)) != SRTP2_OK) {
        LOG_ERR("failed to register SV group for nunchuk acceleration");
        return;
    }

    ready_ = true;
    LOG_INF("application started");
}

void Nunchuk_onInputEvent_(struct input_event* evt, void*) {
    // Ignore any input as long as the application is not ready.
    if (!ready_) return;

    // Handle the different event types.
    switch (evt->type) {
    case INPUT_EV_KEY: {
        uint8_t buttonID = 0;
        switch (evt->code) {
        case INPUT_BTN_Z:
            buttonID = NUNCHUK_Z_BUTTON_ID_;
            break;

        case INPUT_BTN_C:
            buttonID = NUNCHUK_C_BUTTON_ID_;
            break;

        case INPUT_BTN_0:
            buttonID = BTN0_ID_;
            break;

        case INPUT_BTN_1:
            buttonID = BTN1_ID_;
            break;

        case INPUT_BTN_2:
            buttonID = BTN2_ID_;
            break;

        case INPUT_BTN_3:
            buttonID = BTN3_ID_;
            break;

        default:
            buttonID = INVALID_BUTTON_ID;
            LOG_ERR("unknown button event");
        }

        if (buttonID != INVALID_BUTTON_ID) {
            SRTP2_evPublishRequest(evt->value ? EV_BUTTON_PRESSED_ : EV_BUTTON_RELEASED_, &buttonID, 1);
        }
        break;
    }

    case INPUT_EV_ABS:
        switch (evt->code) {
        case INPUT_ABS_X:
            LOG_DBG("X = %u", evt->value);
            joystickPosition_.x = evt->value;
            SRTP2_svPublishUpdate(NUNCHUK_JOYSTICK_SV_GROUP_, &joystickPosition_, sizeof(joystickPosition_));
            break;

        case INPUT_ABS_Y:
            LOG_DBG("Y = %u", evt->value);
            joystickPosition_.y = evt->value;
            SRTP2_svPublishUpdate(NUNCHUK_JOYSTICK_SV_GROUP_, &joystickPosition_, sizeof(joystickPosition_));
            break;

        default:
            LOG_ERR("unknown joystick axis");
        }
        break;

    case INPUT_EV_DEVICE:
        switch (evt->code) {
        case INPUT_NUNCHUK_ACCEL_X:
            acceleration_.x = evt->value;
            break;

        case INPUT_NUNCHUK_ACCEL_Y:
            acceleration_.y = evt->value;
            break;

        case INPUT_NUNCHUK_ACCEL_Z:
            acceleration_.z = evt->value;
            SRTP2_svPublishUpdate(NUNCHUK_ACCELERATION_SV_GROUP_, &acceleration_, sizeof(acceleration_));
            break;
        }
        break;

    default:
        LOG_ERR("unknown input event");
        break;
    }
}

static void Nunchuk_syncIndication_(SRTP2_NetworkID_t, SRTP2_NetworkTime_t, SRTP2_CycleInterval_t, SRTP2_CallbackContext_t) {
    // Trigger the readout of the Nunchuk input device.
    nunchuk_read(nunchukDevice_);
}

// Register global input callback.
INPUT_CALLBACK_DEFINE(NULL, Nunchuk_onInputEvent_, NULL);
