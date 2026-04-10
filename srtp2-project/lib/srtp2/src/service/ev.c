#include <srtp2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2_ev, LOG_LEVEL_INF);

SRTP2_Status_t SRTP2_evPublishRequest(SRTP2_EVId_t eventID, SRTP2_EVPayloadPointer_t payload, SRTP2_EVPayloadLength_t payloadLength) {
    // TODO: implement.

    return SRTP2_ERROR_NOT_IMPLEMENTED;
}
