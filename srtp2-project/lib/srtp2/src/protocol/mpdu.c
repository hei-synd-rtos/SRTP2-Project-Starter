#include "mpdu.h"
#include "offsets.h"
#include <string.h>

SRTP2_MPDU_EPDUCount_t SRTP2_MPDU_ePDUCount(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length) {
    // Check minimal size.
    if (length < SRTP2_MPDU_MINIMAL_LENGHT) return 0;

    // Copy ePDU count from frame.
    SRTP2_MPDU_EPDUCount_t count = 0;
    memcpy(&count, frame + SRTP2_MPDU_EPDU_COUNT_OFFSET, sizeof(count));

    // Return count.
    return count;
}

SRTP2_Status_t SRTP2_MPDU_parse(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length, SRTP2_MPDU_SVCallback_t svCallback,
                                SRTP2_MPDU_EVCallback_t evCallback, SRTP2_MPDU_MSGCallback_t msgCallback) {
    // Check minimal length.
    if (length < SRTP2_MPDU_MINIMAL_LENGHT) return SRTP2_ERROR;

    // If ePDU count is 0, return.
    SRTP2_MPDU_EPDUCount_t count = SRTP2_MPDU_ePDUCount(frame, length);
    if (count == 0) return SRTP2_OK;

    // Decode one ePDU at the time.
    SRTP2_Frame_Pointer_t pos = frame + SRTP2_MPDU_PAYLOAD_OFFSET;
    while (pos < frame + length) {
        uint8_t type = *(pos + SRTP2_EPDU_TYPE_OFFSET);
        switch (type) {
        case SRTP2_EPDU_TYPE_SAMPLED_VALUE: {
            SRTP2_SVGroup_t svGroup = *(pos + SRTP2_SV_EPDU_SVGROUP_OFFSET);
            uint16_t svLength = 0;
            memcpy(&svLength, pos + SRTP2_SV_EPDU_LENGTH_OFFSET, sizeof(svLength));
            if (svCallback) svCallback(svGroup, pos + SRTP2_SV_EPDU_PAYLOAD_OFFSET, svLength);
            pos += SRTP2_SV_EPDU_PAYLOAD_OFFSET + svLength;
            break;
        }

        case SRTP2_EPDU_TYPE_EVENT: {
            SRTP2_EVId_t eventID = 0;
            memcpy(&eventID, pos + SRTP2_EV_EPDU_EVENTID_OFFSET, sizeof(eventID));
            SRTP2_EVTimestamp_t timestamp = 0;
            memcpy(&timestamp, pos + SRTP2_EV_EPDU_TIMESTAMP_OFFSET, sizeof(timestamp));
            uint16_t evLength = 0;
            memcpy(&evLength, pos + SRTP2_EV_EPDU_LENGHT_OFFSET, sizeof(evLength));
            if (evCallback) evCallback(eventID, timestamp, pos + SRTP2_EV_EPDU_PAYLOAD_OFFSET, evLength);
            pos += SRTP2_EV_EPDU_PAYLOAD_OFFSET + evLength;
            break;
        }

        case SRTP2_EPDU_TYPE_MESSAGE: {
            SRTP2_MsgAddress_t destinationAddress = {0};
            memcpy(destinationAddress, pos + SRTP2_MSG_EPDU_DESTINATION_ADDRESS_OFFSET, sizeof(destinationAddress));
            SRTP2_MsgAddress_t sourceAddress = {0};
            memcpy(sourceAddress, frame + FRAME_SOURCE_ADDRESS_OFFSET, sizeof(sourceAddress));
            SRTP2_MsgSAPI_t destinationSAPI = 0;
            memcpy(&destinationSAPI, pos + SRTP2_MSG_EPDU_DESTINATION_SAPI_OFFSET, sizeof(destinationSAPI));
            SRTP2_MsgSAPI_t sourceSAPI = 0;
            memcpy(&sourceSAPI, pos + SRTP2_MSG_EPDU_SOURCE_SAPI_OFFSET, sizeof(sourceSAPI));
            _Bool lastFragment = *(pos + SRTP2_MSG_EPDU_FLAGS_OFFSET) & SRTP2_MSG_EPDU_FLAGS_LAST_FRAGMENT;
            uint16_t fragmentLength = 0;
            memcpy(&fragmentLength, pos + SRTP2_MSG_EPDU_LENGTH_OFFSET, sizeof(fragmentLength));
            if (msgCallback) msgCallback(destinationAddress, sourceAddress, destinationSAPI, sourceSAPI, lastFragment, pos + SRTP2_MSG_EPDU_PAYLOAD_OFFSET,
                                         fragmentLength);
            pos += SRTP2_MSG_EPDU_PAYLOAD_OFFSET + fragmentLength;
            break;
        }

        default:
            return SRTP2_ERROR;
            break;
        }
    }

    return SRTP2_OK;
}

SRTP2_Status_t SRTP2_MPDU_Builder_init(SRTP2_MPDU_Builder_t* builder, SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length) {
    if (builder == NULL || frame == NULL || length < SRTP2_MPDU_MINIMAL_LENGHT) return SRTP2_ERROR;

    // Initialize builder.
    builder->start = frame;
    builder->pos = frame + SRTP2_MPDU_PAYLOAD_OFFSET;
    builder->end = frame + length;

    // Fill in ethernet fields.
    uint16_t etherType = SRTP_ETHERTYPE;
    memset(frame + FRAME_DESTINATION_ADDRESS_OFFSET, 0xFF, FRAME_DESTINATION_ADDRESS_LENGTH);
    memcpy(frame + FRAME_ETHERTYPE_OFFSET, &etherType, sizeof(etherType));

    // Set SRTP2 frame type to MPDU.
    frame[SRTP2_TYPE_OFFSET] = SRTP2_TYPE_MPDU;

    // Set ePDU count to zero.
    uint16_t ePDUCount = 0;
    memcpy(builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, &ePDUCount, sizeof(ePDUCount));

    return SRTP2_OK;
}

SRTP2_Frame_Length_t SRTP2_MPDU_Builder_length(SRTP2_MPDU_Builder_t* builder) {
    if (builder == NULL) return 0;
    return builder->pos - builder->start;
}

SRTP2_Frame_Length_t SRTP2_MPDU_Builder_remaining(SRTP2_MPDU_Builder_t* builder) {
    if (builder == NULL) return 0;
    return builder->end - builder->pos;
}

SRTP2_Status_t SRTP2_MPDU_Builder_addSVData(SRTP2_MPDU_Builder_t* builder, SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadPointer_t payload,
                                            SRTP2_SVPayloadLength_t length) {
    // Check parameters.
    if (builder == NULL || payload == NULL) return SRTP2_ERROR;

    // Check if ePDU can fit into the frame.
    if (builder->pos + SRTP2_SV_EPDU_PAYLOAD_OFFSET + length > builder->end) return SRTP2_ERROR_INSUFFICIENT_CAPACITY;

    // Copy data into the frame.
    *(builder->pos++) = SRTP2_EPDU_TYPE_SAMPLED_VALUE;
    *(builder->pos++) = svGroup;
    memcpy(builder->pos, &length, sizeof(length));
    builder->pos += sizeof(length);
    memcpy(builder->pos, payload, length);
    builder->pos += length;

    // Increment ePDU count.
    uint16_t ePDUCount = 0;
    memcpy(&ePDUCount, builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, sizeof(ePDUCount));
    ++ePDUCount;
    memcpy(builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, &ePDUCount, sizeof(ePDUCount));

    // Return success.
    return SRTP2_OK;
}

SRTP2_Status_t SRTP2_MPDU_Builder_addEVData(SRTP2_MPDU_Builder_t* builder, SRTP2_EVId_t eventID, SRTP2_EVTimestamp_t timestamp,
                                            SRTP2_SVPayloadPointer_t payload, SRTP2_SVPayloadLength_t length) {
    // Check parameters.
    if (builder == NULL || payload == NULL) return SRTP2_ERROR;

    // Check if ePDU can fit into the frame.
    if (builder->pos + SRTP2_EV_EPDU_PAYLOAD_OFFSET + length > builder->end) return SRTP2_ERROR_INSUFFICIENT_CAPACITY;

    // Copy data into frame.
    *(builder->pos++) = SRTP2_EPDU_TYPE_EVENT;
    memcpy(builder->pos, &eventID, sizeof(eventID));
    builder->pos += sizeof(eventID);
    memcpy(builder->pos, &timestamp, sizeof(timestamp));
    builder->pos += sizeof(timestamp);
    memcpy(builder->pos, &length, sizeof(length));
    builder->pos += sizeof(length);
    memcpy(builder->pos, payload, length);
    builder->pos += length;

    // Increment ePDU count.
    uint16_t ePDUCount = 0;
    memcpy(&ePDUCount, builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, sizeof(ePDUCount));
    ++ePDUCount;
    memcpy(builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, &ePDUCount, sizeof(ePDUCount));

    // Return success.
    return SRTP2_OK;
}

SRTP2_Status_t SRTP2_MPDU_Builder_addMSGData(SRTP2_MPDU_Builder_t* builder, SRTP2_MsgAddress_t destinationAddress, SRTP2_MsgSAPI_t destinationSAPI,
                                             SRTP2_MsgSAPI_t sourceSAPI, _Bool lastFragment, SRTP2_MsgPayloadPointer_t payload,
                                             SRTP2_MsgPayloadLength_t length) {
    // Check parameters.
    if (builder == NULL || payload == NULL) return SRTP2_ERROR;

    // Check if ePDU can fit into the frame.
    if (builder->pos + SRTP2_MSG_EPDU_PAYLOAD_OFFSET + length > builder->end) return SRTP2_ERROR_INSUFFICIENT_CAPACITY;

    // Copy data into frame.
    *(builder->pos++) = SRTP2_EPDU_TYPE_MESSAGE;
    memcpy(builder->pos, destinationAddress, SRTP2_MSG_EPDU_DESTINATION_ADDRESS_LENGTH);
    builder->pos += SRTP2_MSG_EPDU_DESTINATION_ADDRESS_LENGTH;
    memcpy(builder->pos, &destinationSAPI, sizeof(destinationSAPI));
    builder->pos += sizeof(destinationSAPI);
    memcpy(builder->pos, &sourceSAPI, sizeof(sourceSAPI));
    builder->pos += sizeof(sourceSAPI);
    *(builder->pos++) = lastFragment ? SRTP2_MSG_EPDU_FLAGS_LAST_FRAGMENT : 0;
    memcpy(builder->pos, &length, sizeof(length));
    builder->pos += sizeof(length);
    memcpy(builder->pos, payload, length);
    builder->pos += length;

    // Increment ePDU count.
    uint16_t ePDUCount = 0;
    memcpy(&ePDUCount, builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, sizeof(ePDUCount));
    ++ePDUCount;
    memcpy(builder->start + SRTP2_MPDU_EPDU_COUNT_OFFSET, &ePDUCount, sizeof(ePDUCount));

    // Return success.
    return SRTP2_OK;
}
