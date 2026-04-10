#pragma once
#include <srtp2/types.h>
#include "frame.h"


/**
 * @brief Number of ePDUs in an SRTP2 MPDU frame.
 */
typedef uint16_t SRTP2_MPDU_EPDUCount_t;

/**
 * @brief Get the number of ePDUs in an SRTP2 MPDU frame.
 *
 * @param frame     Pointer to the Ethernet frame.
 * @param length    Length of the Ethernet frame.
 * @return          The number of ePDUs contained in the MPDU or 0 if the frame is invalid.
 */
SRTP2_MPDU_EPDUCount_t SRTP2_MPDU_ePDUCount(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length);

/**
 * @brief Callback for processing Sampled Value (SV) ePDU data.
 *
 * This callback is called by the SRTP2MPDU_parse() function for each sampled values (SV) ePDU in the MPDU frame.
 *
 * @param svGroup   The SV group the payload belongs to.
 * @param payload   Pointer to the SV payload data.
 * @param length    Length of the SV payload.
 */
typedef void (*SRTP2_MPDU_SVCallback_t)(SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadPointer_t payload, SRTP2_SVPayloadLength_t length);

/**
 * @brief Callback for processing Event (EV) ePDU data.

 * This callback is called by the SRTP2MPDU_parse() function for each event (EV) ePDU in the MPDU frame.
 *
 * @param eventID       The unique identifier of the event.
 * @param timestamp     The network timestamp when the event occurred.
 * @param length        Length of the event payload.
 * @param payload       Pointer to the event payload data.
 */
typedef void (*SRTP2_MPDU_EVCallback_t)(SRTP2_EVId_t eventID, SRTP2_EVTimestamp_t timestamp, SRTP2_EVPayloadPointer_t payload, SRTP2_EVPayloadLength_t length);

/**
 * @brief Callback for processing Message (MSG) ePDU data.

 * This callback is called by the SRTP2MPDU_parse() function for each message (MSG) ePDU in the MPDU frame.
 *
 * @param destinationAddress The MAC address of the destination.
 * @param sourceAddress      The MAC address of the source.
 * @param destinationSAPI    The Service Access Point Identifier of the destination.
 * @param sourceSAPI         The Service Access Point Identifier of the source.
 * @param lastFragment       Boolean indicating if this is the last fragment of the message.
 * @param length             Length of the message fragment payload.
 * @param payload            Pointer to the message fragment payload data.
 */
typedef void (*SRTP2_MPDU_MSGCallback_t)(SRTP2_MsgAddress_t destinationAddress, SRTP2_MsgAddress_t sourceAddress, SRTP2_MsgSAPI_t destinationSAPI,
                                         SRTP2_MsgSAPI_t sourceSAPI, _Bool lastFragment, SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length);

/**
 * @brief Parse an SRTP2 MPDU frame and invoke callbacks for each contained ePDU.

 * This function iterates through all ePDUs (SV, EV, MSG) within an MPDU frame and
 * calls the corresponding callback function for each one found.
 *
 * @param frame       Pointer to the Ethernet frame.
 * @param length      Length of the Ethernet frame.
 * @param svCallback  Callback function for Sampled Value ePDUs. Can be NULL.
 * @param evCallback  Callback function for Event ePDUs. Can be NULL.
 * @param msgCallback Callback function for Message ePDUs. Can be NULL.
 * @return            SRTP2_OK if the frame was parsed successfully, SRTP2_ERROR otherwise.
 */
SRTP2_Status_t SRTP2_MPDU_parse(SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length, SRTP2_MPDU_SVCallback_t svCallback,
                               SRTP2_MPDU_EVCallback_t evCallback, SRTP2_MPDU_MSGCallback_t msgCallback);

typedef struct {
    SRTP2_Frame_Pointer_t start, end, pos;
} SRTP2_MPDU_Builder_t;

SRTP2_Status_t SRTP2_MPDU_Builder_init(SRTP2_MPDU_Builder_t* builder, SRTP2_Frame_Pointer_t frame, SRTP2_Frame_Length_t length);

SRTP2_Frame_Length_t SRTP2_MPDU_Builder_length(SRTP2_MPDU_Builder_t* builder);

SRTP2_Frame_Length_t SRTP2MPDU_Builder_remaining(SRTP2_MPDU_Builder_t* builder);

SRTP2_Status_t SRTP2_MPDU_Builder_addSVData(SRTP2_MPDU_Builder_t* builder, SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadPointer_t payload,
                                           SRTP2_SVPayloadLength_t length);

SRTP2_Status_t SRTP2_MPDU_Builder_addEVData(SRTP2_MPDU_Builder_t* builder, SRTP2_EVId_t eventID, SRTP2_EVTimestamp_t timestamp,
                                            SRTP2_EVPayloadPointer_t payload, SRTP2_EVPayloadLength_t length);

SRTP2_Status_t SRTP2_MPDU_Builder_addMSGData(SRTP2_MPDU_Builder_t* builder, SRTP2_MsgAddress_t destinationAddress, SRTP2_MsgSAPI_t destinationSAPI,
                                             SRTP2_MsgSAPI_t sourceSAPI, _Bool lastFragment, SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length);
