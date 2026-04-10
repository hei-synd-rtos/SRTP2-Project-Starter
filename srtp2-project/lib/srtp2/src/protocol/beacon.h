#pragma once
#include <srtp2/types.h>
#include "frame.h"


/**
 * @brief Parse an SRTP2 Beacon frame.
 *
 * This function extracts the network parameters from a received Beacon frame.
 *
 * @param framePtr          Pointer to the Ethernet frame.
 * @param length            Length of the Ethernet frame.
 * @param networkID         Pointer to store the extracted Network ID. Can be NULL if not needed.
 * @param networkTime       Pointer to store the extracted network time. Can be NULL if not needed.
 * @param cycleInterval     Pointer to store the extracted cycle interval. Can be NULL if not needed.
 * @param svGroupMask       Pointer to store the extracted SV group mask. Can be NULL if not needed.
 * @return                  SRTP2_STATUS_OK if successful, or an error code if the frame is invalid.
 */
SRTP2_Status_t SRTP2_Beacon_parse(SRTP2_Frame_Pointer_t framePtr, SRTP2_Frame_Length_t length, SRTP2_NetworkID_t networkID,
                                  SRTP2_NetworkTime_t* networkTime, SRTP2_CycleInterval_t* cycleInterval, SRTP2_SVGroupMask_t* svGroupMask);

/**
 * @brief Build an SRTP2 Beacon frame.

 * This function constructs a Beacon frame with the provided network parameters.
 *
 * @param frame             Pointer to the buffer where the frame will be built.
 * @param maxLength         Maximum length of the buffer.
 * @param networkID         The Network ID to include in the Beacon.
 * @param networkTime       The current network time.
 * @param cycleInterval     The cycle interval of the network.
 * @param svGroupMask       The mask of active SV groups.
 * @return                  The length of the built frame, or 0 if the buffer is too small.

 */
SRTP2_Frame_Length_t SRTP2_Beacon_build(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t maxLength, SRTP2_NetworkID_t networkID,
                                        SRTP2_NetworkTime_t networkTime, SRTP2_CycleInterval_t cycleInterval, SRTP2_SVGroupMask_t svGroupMask);
