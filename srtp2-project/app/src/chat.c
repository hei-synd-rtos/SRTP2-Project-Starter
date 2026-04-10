#include "chat.h"
#include <srtp2.h>
#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(chat, LOG_LEVEL_INF);

static const struct gpio_dt_spec ld3_ = GPIO_DT_SPEC_GET(DT_ALIAS(extled3), gpios);

static const uint16_t SAPI_ = 1;
enum {
    MAX_MESSAGE_SIZE_ = 512,
};

static int Chat_command_(const struct shell* sh, size_t argc, char** argv);
static void Chat_messageConfirmation_(SRTP2_Status_t status, SRTP2_CallbackContext_t context);
static void Chat_messageIndication_(SRTP2_MsgAddress_t sourceAddress, SRTP2_MsgSAPI_t destinationSapi, SRTP2_MsgSAPI_t sourceSapi,
    SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length, SRTP2_CallbackContext_t context);

void Chat_init() {
    // Check if the GPIO is ready.
    if (!gpio_is_ready_dt(&ld3_)) {
        LOG_ERR("GPIO not ready");
        return;
    }

    // Configure the GPIO.
    gpio_pin_configure_dt(&ld3_, GPIO_OUTPUT_ACTIVE);
    gpio_pin_set_dt(&ld3_, 0);

    // Register chat message listener.
    if (SRTP2_msgListenRequest(SAPI_, Chat_messageIndication_, NULL) != SRTP2_OK) {
        LOG_ERR("failed to register message listener");
    }

    // Register shell command "chat".
    SHELL_CMD_ARG_REGISTER(chat, NULL, "Send a chat message over SRTP2.\n" "Usage: chat <mac> <\"Text Message\">", Chat_command_, 3, 3);

    LOG_INF("application started");
}

static int Chat_command_(const struct shell* sh, size_t argc, char** argv) {
    // Check that we have exactly 3 arguments, the command, the destination MAC address and the message.
    if (argc != 3) {
        shell_error(sh, "Usage: chat <mac> <\"Text Message\">");
        return -EINVAL;
    }

    // Parse the MAC Address
    uint8_t mac[6];
    if (sscanf(argv[1], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6) {
        shell_error(sh, "Invalid MAC address format. Please use XX:XX:XX:XX:XX:XX");
        return -EINVAL;
    }

    // Send message.
    SRTP2_msgDataRequest(mac, SAPI_, SAPI_, argv[2], strlen(argv[2]), Chat_messageConfirmation_, NULL);

    return 0;
}

static void Chat_messageConfirmation_(SRTP2_Status_t status, SRTP2_CallbackContext_t context) {
    if (status == SRTP2_OK) {
        LOG_INF("message sent successfully");
    }
    else {
        LOG_ERR("failed to send message");
    }
}

static void Chat_messageIndication_(SRTP2_MsgAddress_t sourceAddress, SRTP2_MsgSAPI_t destinationSapi, SRTP2_MsgSAPI_t sourceSapi,
    SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length, SRTP2_CallbackContext_t context) {

    // Copy message to a null-terminated buffer.
    static char buffer[MAX_MESSAGE_SIZE_ + 1];
    memcpy(buffer, payload, length);
    buffer[length] = '\0';

    // Notify user that the message was received.
    LOG_INF("Received Message from %d: %s", sourceAddress[5], buffer);

    // Handle LED commands.
    if (strncmp(payload, "led on", length) == 0) {
        gpio_pin_set_dt(&ld3_, true);
    }
    else if (strncmp(payload, "led off", length) == 0) {
        gpio_pin_set_dt(&ld3_, false);
    }
    else if (strncmp(payload, "led toggle", length) == 0) {
        gpio_pin_toggle_dt(&ld3_);
    }
}
