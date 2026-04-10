#include <srtp2.h>
#include <cmsis_os2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2_sync, LOG_LEVEL_INF);

SRTP2_Status_t SRTP2_syncRequest(SRTP2_SyncIndicationCallback_t callback, SRTP2_CallbackContext_t callbackContext) {
    // TODO: Implement.
    return SRTP2_ERROR_NOT_IMPLEMENTED;
}

void SRTP2_syncThread_(void*) {
    LOG_DBG("starting sync thread");

    while (true) {
        // TODO: Implement.
    }
}
