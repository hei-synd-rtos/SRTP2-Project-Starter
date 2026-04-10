#include "beacon.h"
#include "offsets.h"
#include <string.h>

SRTP2_Status_t SRTP2_Beacon_parse(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length, SRTP2_NetworkID_t networkID,
                                  SRTP2_NetworkTime_t* networkTime, SRTP2_CycleInterval_t* cycleInterval, SRTP2_SVGroupMask_t* svGroupMask) {
    // The length of a Beacon is fixed.
    if (length != SRTP2_BEACON_LENGTH) return SRTP2_ERROR;

    // Store network ID if requested.
    if (networkID != NULL) memcpy(networkID, frame + SRTP2_BEACON_NETWORK_ID_OFFSET, SRTP2_BEACON_NETWORK_ID_SIZE);

    // Store network time if requested.
    if (networkTime != NULL) memcpy(networkTime, frame + SRTP2_BEACON_NETWORK_TIME_OFFSET, sizeof(SRTP2_NetworkTime_t));

    // Store cycle interval if requested.
    if (cycleInterval != NULL) memcpy(cycleInterval, frame + SRTP2_BEACON_CYCLE_INTERVAL_OFFSET, sizeof(SRTP2_CycleInterval_t));

    // Store SV group mask if requested.
    if (svGroupMask != NULL) memcpy(svGroupMask, frame + SRTP2_BEACON_SV_GROUP_MASK_OFFSET, sizeof(SRTP2_SVGroupMask_t));

    return SRTP2_OK;
}

SRTP2_Frame_Length_t SRTP2_Beacon_build(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t maxLength, SRTP2_NetworkID_t networkID,
                                        SRTP2_NetworkTime_t networkTime, SRTP2_CycleInterval_t cycleInterval, SRTP2_SVGroupMask_t svGroupMask) {
    // Check minimal length.
    if (maxLength < SRTP2_BEACON_LENGTH) return 0;

    // Copy network ID.
    memcpy(frame + SRTP2_BEACON_NETWORK_ID_OFFSET, networkID, SRTP2_BEACON_NETWORK_ID_SIZE);

    // Copy network time.
    memcpy(frame + SRTP2_BEACON_NETWORK_TIME_OFFSET, &networkTime, sizeof(networkTime));

    // Copy cycle interval.
    memcpy(frame + SRTP2_BEACON_CYCLE_INTERVAL_OFFSET, &cycleInterval, sizeof(cycleInterval));

    // Copy SV group mask.
    memcpy(frame + SRTP2_BEACON_SV_GROUP_MASK_OFFSET, &svGroupMask, sizeof(svGroupMask));

    // Return length.
    return SRTP2_BEACON_LENGTH;
}
