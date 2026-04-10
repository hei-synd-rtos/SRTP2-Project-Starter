#include <srtp2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2_sv, LOG_LEVEL_INF);

SRTP2_Status_t SRTP2_svPublishRequest(SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadLength_t payloadLength) {
    // TODO: implement.
    return SRTP2_ERROR_NOT_IMPLEMENTED;;
}

SRTP2_Status_t SRTP2_svPublishUpdate(SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadPointer_t payload, SRTP2_SVPayloadLength_t payloadLength) {
    // TODO: Implement.
    return SRTP2_ERROR_NOT_IMPLEMENTED;
}
