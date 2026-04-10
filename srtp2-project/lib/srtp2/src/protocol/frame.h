#pragma once
#include <stdint.h>
#include <stddef.h>

enum {
#ifdef CONFIG_SRTP2_FRAME_MTU
    SRTP2_FRAME_MTU = CONFIG_SRTP2_FRAME_MTU
#else
    SRTP2_FRAME_MTU = 1530
#endif
};

/**
 * @brief Enumeration of SRTP2 frame types.
 */
typedef enum {
    /**
     * @brief The frame is not a valid SRTP2 frame.
     */
    SRTP2_FRAME_TYPE_INVALID,

    /**
     * @brief The frame is an SRTP2 Beacon frame.
     */
    SRTP2_FRAME_TYPE_BEACON,

    /**
     * @brief The frame is an SRTP2 MPDU frame.
     */
    SRTP2_FRAME_TYPE_MPDU
} SRTP2_Frame_Type_t;

/**
 * @brief Pointer to an Ethernet frame buffer.
 */
typedef uint8_t* SRTP2_Frame_Pointer_t;

/**
 * @brief Length of an Ethernet frame.
 */
typedef size_t SRTP2_Frame_Length_t;

/**
 * @brief Get the type of the SRTP2 frame.
 *
 * @param framePtr  Pointer to the Ethernet frame.
 * @param length    Length of the Ethernet frame.
 * @return          The type of the SRTP2 frame or SRTP2_FRAME_TYPE_INVALID if the frame is invalid or not a SRTP2 frame.
 */
SRTP2_Frame_Type_t SRTP2_Frame_type(SRTP2_Frame_Pointer_t framePtr, SRTP2_Frame_Length_t length);
