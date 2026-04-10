#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @addtogroup srtp2
 * @{
 */

/**
 * @brief Status codes for SRTP2 operations.
 */
typedef enum {
    SRTP2_OK = 0,                               //< Operation was successful.
    SRTP2_ERROR = -1,                           //< An unspecified error occurred.
    SRTP2_ERROR_INSUFFICIENT_CAPACITY = -2,     //< Insufficient capacity to complete the operation.
    SRTP2_ERROR_CONFLICT = -3,                  //< A conflict occurred (e.g., resource already registered).
    SRTP2_ERROR_INSUFFICIENT_MEMORY = -4,       //< Insufficient memory to complete the operation.
    SRTP2_ERROR_NOT_FOUND = -5,                 //< Resource not found.
    SRTP2_ERROR_NOT_IMPLEMENTED = -6,           //< Operation isn't implemented or supported.
    SRTP2_ERROR_INVALID_ARGUMENT = -7           //< Invalid argument provided.
} SRTP2_Status_t;

/**
 * @brief User-provided context passed to callback functions.
 */
typedef void* SRTP2_CallbackContext_t;

/**
 * @brief Pointer to a network interface structure used for SRTP2 communication.
 */
typedef struct net_if* SRTP2_NetworkDevice_t;

/**
 * @brief Pointer to a network identifier string.
 */
typedef unsigned char* SRTP2_NetworkID_t;

/**
 * @brief Represents the synchronized network time in microseconds since the UNIX epoch.
 */
typedef uint64_t SRTP2_NetworkTime_t;

/**
 * @brief Represents the duration of a communication cycle in microseconds.
 */
typedef uint32_t SRTP2_CycleInterval_t;

/**
 * @brief Represents a Sampled Values (SV) group identifier.
 */
typedef uint8_t SRTP2_SVGroup_t;

/**
 * @brief Represents a bitmask of active Sampled Values (SV) groups.
 */
typedef uint32_t SRTP2_SVGroupMask_t;

/**
 * Converts a status variable (SV) group identifier to a corresponding group mask.
 *
 * @param svGroup The status variable group identifier to be converted.
 * @return The group mask corresponding to the provided status variable group identifier.
 */
inline SRTP2_SVGroupMask_t SRTP2_SV_GROUP_TO_MASK(SRTP2_SVGroup_t svGroup) {
    return 1 << svGroup;
}

/**
 * Checks if a specific Sampled Value (SV) group is active based on the given SV group mask.
 *
 * This function determines the active status of an SV group by masking its corresponding
 * bit in the provided SV group mask.
 *
 * @param svGroup The sampled value group to check. This is represented as an 8-bit value.
 * @param svGroupMask The sampled value group mask, represented as a 32-bit value, which
 *        contains activation states of SV groups as bit flags.
 * @return A boolean value (_Bool) indicating whether the specified SV group is active.
 *         Returns true if the SV group is active, otherwise false.
 */
inline _Bool SRTP2_SV_GROUP_IS_ACTIVE(SRTP2_SVGroup_t svGroup, SRTP2_SVGroupMask_t svGroupMask) {
    return (svGroupMask & SRTP2_SV_GROUP_TO_MASK(svGroup)) != 0;
}

/**
 * @brief Represents the length of a Sampled Values (SV) payload in bytes.
 */
typedef uint16_t SRTP2_SVPayloadLength_t;

/**
 * @brief Pointer to a Sampled Values (SV) payload data buffer.
 */
typedef void* SRTP2_SVPayloadPointer_t;

/**
 * @brief Represents a 6-byte MAC address used for a SRTP2 sampled values source.
 */
typedef uint8_t SRTP2_SVSourceAddress_t[6];

/**
 * @brief Represents an Event (EV) identifier.
 */
typedef uint16_t SRTP2_EVId_t;

/**
 * @brief Represents the timestamp of an Event (EV) in microseconds since UNIX epoch.
 */
typedef uint64_t SRTP2_EVTimestamp_t;

/**
 * @brief Represents the length of an Event (EV) payload in bytes.
 */
typedef uint16_t SRTP2_EVPayloadLength_t;

/**
 * @brief Pointer to an Event (EV) payload data buffer.
 */
typedef void* SRTP2_EVPayloadPointer_t;

/**
 * @brief Represents a 6-byte MAC address used for a SRTP2 event source.
 */
typedef uint8_t SRTP2_EVSourceAddress_t[6];

/**
 * @brief Represents a 6-byte MAC address used for SRTP2 message addressing.
 */
typedef uint8_t SRTP2_MsgAddress_t[6];

/**
 * @brief Represents a Service Access Point Identifier (SAPI) for SRTP2 messages.
 */
typedef uint16_t SRTP2_MsgSAPI_t;

/**
 * @brief Represents the length of a Message (MSG) payload in bytes.
 */
typedef uint16_t SRTP2_MsgPayloadLength_t;

/**
 * @brief Pointer to a Message (MSG) payload data buffer.
 */
typedef void* SRTP2_MsgPayloadPointer_t;

/** @} */
