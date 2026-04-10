#include "frame.h"
#include "offsets.h"
#include <string.h>

SRTP2_Frame_Type_t SRTP2_Frame_type(SRTP2_Frame_Pointer_t framePtr, SRTP2_Frame_Length_t length) {
    // Check the frame pointer and the minimal frame length.
    if (framePtr == NULL || length < SRTP2_MPDU_MINIMAL_LENGHT) return SRTP2_FRAME_TYPE_INVALID;

    // Check the destination address (must be the broadcast address).
    for (size_t i = FRAME_DESTINATION_ADDRESS_OFFSET; i < FRAME_DESTINATION_ADDRESS_LENGTH; ++i) {
        if (framePtr[i] != 0xFF) return SRTP2_FRAME_TYPE_INVALID;
    }

    // Check Ether type.
    uint16_t etherType;
    memcpy(&etherType, framePtr + FRAME_ETHERTYPE_OFFSET, sizeof(etherType));
    if (etherType != SRTP_ETHERTYPE) return SRTP2_FRAME_TYPE_INVALID;

    // Check frame type and size.
    switch (framePtr[SRTP2_TYPE_OFFSET]) {
    case SRTP2_TYPE_BEACON:
        // Beacon frame has a fixed size of 63 bytes.
        if (length != SRTP2_BEACON_LENGTH) return SRTP2_FRAME_TYPE_INVALID;
        return SRTP2_FRAME_TYPE_BEACON;
    case SRTP2_TYPE_MPDU:
        // MPDU frame has a minimum size of 17 bytes.
        return SRTP2_FRAME_TYPE_MPDU;
    default:
        return SRTP2_FRAME_TYPE_INVALID;
    }
}
