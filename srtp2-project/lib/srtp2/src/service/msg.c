#include <srtp2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2_msg, LOG_LEVEL_INF);

SRTP2_Status_t SRTP2_msgDataRequest(SRTP2_MsgAddress_t destinationAddress, SRTP2_MsgSAPI_t destinationSapi, SRTP2_MsgSAPI_t sourceSapi,
                                    SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length, SRTP2_MsgDataConfirmationCallback_t callback,
                                    SRTP2_CallbackContext_t callbackContext) {
    // TODO: implement.

    return SRTP2_ERROR_NOT_IMPLEMENTED;
}

SRTP2_Status_t SRTP2_msgListenRequest(SRTP2_MsgSAPI_t sapi, SRTP2_MsgIndicationCallback_t callback, SRTP2_CallbackContext_t callbackContext) {
    // TODO: Implement.

    return SRTP2_ERROR_NOT_IMPLEMENTED;
}

void SRTP2_msgThread_(void*) {
    LOG_DBG("starting msg thread");

    while (true) {
        // TODO: Implement.
    }
}
