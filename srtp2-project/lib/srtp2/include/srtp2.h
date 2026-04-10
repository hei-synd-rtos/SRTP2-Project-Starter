#pragma once
#include "srtp2/types.h"

/***************************************************************************************************************************************************************
 * @defgroup srtp2 SRTP2
 * @brief SRTP2 protocol implementation for Zephyr.
 *
 * @addtogroup srtp2
 * @{
 */

/**
 * @brief Initializes the SRTP2 protocol stack for operation.
 *
 * This function sets up the networking stack, initializes services such as synchronization, SV (Sampled Values) handling, EV (Event) handling, and Msg
 * (Message) handling, and starts threads for frame transmission and reception. It ensures no further initialization occurs if the protocol stack is already
 * initialized.
 *
 * @param networkDevice The network interface to be used for SRTP2 communication. Must be provided and valid.
 *
 * @return SRTP2_OK if the initialization was successful.
 * @return SRTP2_ERROR if an error occurred during initialization (e.g., socket creation failed, mutex allocation failed, etc.).
 */
SRTP2_Status_t SRTP2_init(SRTP2_NetworkDevice_t networkDevice);


/***************************************************************************************************************************************************************
 * @defgroup sync
 * @brief The Synchronization service provides a mechanism for all nodes in the SRTP2 network to synchronize their local clocks to a common network time. This
 * is achieved through the periodic reception of Beacon frames, which trigger a synchronization event. Applications can register callbacks to be notified
 * whenever a new synchronization point is reached.
 *
 * @addtogroup sync
 * @{
 */

/**
 * @brief Callback function type for synchronization indications.
 *
 * This callback is invoked whenever a synchronization event (e.g., a Beacon frame) is received, providing the current network identity, time, and cycle
 * interval.
 *
 * @param networkID The unique identifier of the SRTP2 network.
 * @param networkTime The current synchronized network time.
 * @param cycleInterval The duration of the current communication cycle.
 * @param context User-provided context passed during registration.
 */
typedef void (*SRTP2_SyncIndicationCallback_t)(SRTP2_NetworkID_t networkID, SRTP2_NetworkTime_t networkTime, SRTP2_CycleInterval_t cycleInterval,
                                               SRTP2_CallbackContext_t context);

/**
 * @brief Registers a sync indication callback for SRTP2 synchronization events.
 *
 * This function allows a user application to subscribe to synchronization events in SRTP2 by providing a callback. The callback will be invoked
 * whenever a sync event occurs, delivering relevant synchronization information. The function ensures thread safety by using a mutex to manage
 * access to subscription records.
 *
 * @param callback The callback function to be registered for sync indications.
 *                 Must not be NULL.
 * @param callbackContext Contextual data to be passed to the callback function
 *                        when it is triggered.
 *
 * @return SRTP2_OK if the callback was registered successfully.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if the callback is NULL.
 * @return SRTP2_ERROR_INSUFFICIENT_CAPACITY if the maximum number of sync subscriptions has been reached.
 */
SRTP2_Status_t SRTP2_syncRequest(SRTP2_SyncIndicationCallback_t callback, SRTP2_CallbackContext_t callbackContext);

/** @} */


/***************************************************************************************************************************************************************
 * @defgroup sv SRTP2 Sampled Values service
 * @brief The Sampled Values (SV) service provides a mechanism for the periodic exchange of digitized sensor data or other time-critical information.
 * Nodes can publish data to specific SV groups, which are then broadcast during the designated transmission phase. Other nodes can subscribe to these
 * groups to receive the data via registered callbacks.
 *
 * @addtogroup sv
 * @{
 */

/**
 * @brief Registers a request to publish Sampled Values (SV) for a specific group.
 *
 * This function verifies the provided SV group and payload length, and attempts to register the group for publishing. It ensures that the SV group is not
 * already registered and that sufficient capacity is available for the registration.
 *
 * @param svGroup The SV group to be registered for publishing. Must be a valid group within the supported range.
 * @param payloadLength The length of the payload to be published. Must be less than or equal to the maximum allowed payload length (SV_MAX_PAYLOAD_LENGTH).
 *
 * @return SRTP2_OK if the SV group was successfully registered.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if the provided arguments are invalid (e.g., an out-of-range group or payload length).
 * @return SRTP2_ERROR_CONFLICT if the SV group is already registered by another application.
 * @return SRTP2_ERROR_INSUFFICIENT_CAPACITY if there is no available capacity for additional registrations.
 */
SRTP2_Status_t SRTP2_svPublishRequest(SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadLength_t payloadLength);

/**
 * @brief Updates the Sampled Values (SV) payload associated with a specific SV group.
 *
 * This function locates the SV group specified by the provided group identifier and updates its payload with the data pointed to by the payload argument. If
 * the specified group does not exist or the payload length is invalid, an appropriate error status is returned. The function ensures thread safety using a
 * mutex during updates.
 *
 * @param svGroup The identifier of the SV group to update. Must be a valid group identifier.
 * @param payload Pointer to the new payload data. Must not be null and must point to a valid memory location.
 * @param payloadLength The length of the new payload data. Must not exceed the maximum allowed payload length.
 *
 * @return SRTP2_OK if the payload was successfully updated.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if the SV group or payload length is invalid.
 * @return SRTP2_ERROR_NOT_FOUND if the specified SV group does not exist.
 */
SRTP2_Status_t SRTP2_svPublishUpdate(SRTP2_SVGroup_t svGroup, SRTP2_SVPayloadPointer_t payload, SRTP2_SVPayloadLength_t payloadLength);

/** @} */


/***************************************************************************************************************************************************************
 * @defgroup ev SRTP2 Event service
 * @brief The Event (EV) service provides a mechanism for the transmission of spontaneous, time-stamped events. Events are queued and transmitted during the
 * next available transmission phase. This service is suitable for non-periodic notifications such as alarms, status changes, or user inputs.
 *
 * @addtogroup ev
 * @{
 */

/**
 * @brief Publishes an event with a specified payload and length to the event queue.
 *
 * This function creates an event with the provided event identifier, payload, and payload length, then enqueues it into the system's event queue for
 * processing. Ensures proper memory allocation and validates arguments for payload and length.
 *
 * @param eventID The identifier of the event to be published.
 * @param payload A pointer to the payload data associated with the event. Must not be NULL.
 * @param payloadLength The length of the payload data in bytes. Must not exceed the maximum payload length.
 *
 * @return SRTP2_OK if the event was successfully published to the queue.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if the payload is NULL or the payload length exceeds the maximum permitted length.
 * @return SRTP2_ERROR_INSUFFICIENT_MEMORY if memory allocation for the event fails.
 * @return SRTP2_ERROR_INSUFFICIENT_CAPACITY if the event queue is full and cannot accept new events.
 */
SRTP2_Status_t SRTP2_evPublishRequest(SRTP2_EVId_t eventID, SRTP2_EVPayloadPointer_t payload, SRTP2_EVPayloadLength_t payloadLength);

/**
 * @brief Callback function type for Event (EV) indications.
 *
 * This callback is invoked whenever an event for a subscribed event ID is received.
 *
 * @param eventID The identifier of the received event.
 * @param sourceAddress The source address of the event.
 * @param payload Pointer to the received event payload data.
 * @param payloadLength The length of the received payload data.
 * @param context User-provided context passed during registration.
 */
typedef void (*SRTP2_EventIndicationCallback_t)(SRTP2_EVId_t eventID, SRTP2_EVSourceAddress_t sourceAddress, SRTP2_EVPayloadPointer_t payload,
                                                SRTP2_EVPayloadLength_t payloadLength, SRTP2_CallbackContext_t context);

/** @} */


/***************************************************************************************************************************************************************
 * @defgroup msg SRTP2 Message service
 * @brief The Message (MSG) service provides a connectionless, unconfirmed or confirmed data exchange mechanism between nodes.  It uses Service Access
 * Points (SAPs) to multiplex different applications or services on the same node. This service is typically used for configuration, diagnostics, or other
 * non-time-critical communication.
 *
 * @addtogroup msg
 * @{
 */

/**
 * @brief Callback function type for Message (MSG) data confirmations.
 *
 * This callback is invoked to provide the status of a previously requested message transmission.
 *
 * @param status The result of the message transmission operation.
 * @param context User-provided context passed during the data request.
 */
typedef void (*SRTP2_MsgDataConfirmationCallback_t)(SRTP2_Status_t status, SRTP2_CallbackContext_t context);

/**
 * @brief Sends a data message request to the specified destination address using the SRTP2 protocol.
 *
 * This function facilitates the transmission of a message by specifying the destination address, service access points (SAPIs),
 * the payload, and a callback for confirmation upon message completion. It ensures the message is dispatched correctly and notifies
 * the caller of the operation's outcome via the callback mechanism.
 *
 * @param destinationAddress The address of the destination device where the message should be sent. Must be a valid and correctly formatted address.
 * @param destinationSapi The SAPI for the destination service. Determines the service to which the message is directed on the receiving side.
 * @param sourceSapi The SAPI for the source service. Identifies the service from which the message originates.
 * @param payload Pointer to the payload data to be sent. Must be valid and allocated by the caller.
 * @param length Length of the message payload in bytes. Must match the limits and requirements of the protocol.
 * @param callback Function pointer to the confirmation callback, invoked upon completion or failure of the message delivery.
 * @param callbackContext Context information to be passed to the callback. Can be used to provide user-specific metadata or state.
 *
 * @return SRTP2_OK if the message was successfully transmitted.
 * @return SRTP2_ERROR if an unspecified error occurred.
 * @return SRTP2_ERROR_INSUFFICIENT_CAPACITY if there is insufficient capacity in buffers or resources to handle the request.
 * @return SRTP2_ERROR_CONFLICT if there is a conflict preventing the message from being sent.
 * @return SRTP2_ERROR_INSUFFICIENT_MEMORY if memory allocation failed.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if any of the provided arguments are invalid.
 * @return SRTP2_ERROR_NOT_IMPLEMENTED if the requested operation is not implemented or supported.
 */
SRTP2_Status_t SRTP2_msgDataRequest(SRTP2_MsgAddress_t destinationAddress, SRTP2_MsgSAPI_t destinationSapi, SRTP2_MsgSAPI_t sourceSapi,
                                    SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length, SRTP2_MsgDataConfirmationCallback_t callback,
                                    SRTP2_CallbackContext_t callbackContext);

/**
 * @brief Callback function type for Message (MSG) indications.

 * This callback is invoked whenever a message for a registered SAPI is received.
 *
 * @param sourceAddress The address of the device that sent the message.
 * @param destinationSapi The SAPI on the local device the message was addressed to.
 * @param sourceSapi The SAPI on the remote device the message originated from.
 * @param payload Pointer to the received message payload data.
 * @param length The length of the received payload data.
 * @param context User-provided context passed during registration.
 */
typedef void (*SRTP2_MsgIndicationCallback_t)(SRTP2_MsgAddress_t sourceAddress, SRTP2_MsgSAPI_t destinationSapi, SRTP2_MsgSAPI_t sourceSapi,
                                              SRTP2_MsgPayloadPointer_t payload, SRTP2_MsgPayloadLength_t length, SRTP2_CallbackContext_t context);

/**
 * @brief Registers a callback to listen for incoming SRTP2 messages on a specified Service Access Point Identifier (SAPI).
 *
 * This function allows the user to register a message indication callback, which is invoked when an SRTP2 message is received on the specified SAPI.
 *
 * @param sapi The Service Access Point Identifier (SAPI) on which to listen for incoming messages.
 * @param callback The function to call when an SRTP2 message is received. This callback receives details of the incoming message.
 * @param callbackContext A user-defined context to pass to the callback function when it is invoked.
 *
 * @return SRTP2_OK if the operation is successful.
 * @return SRTP2_ERROR_INSUFFICIENT_CAPACITY if the system has no capacity left to register more listeners.
 * @return SRTP2_ERROR_CONFLICT if the specified SAPI is already registered for listening.
 * @return SRTP2_ERROR_INVALID_ARGUMENT if any of the input parameters are invalid.
 * @return SRTP2_ERROR_INSUFFICIENT_MEMORY if there is insufficient memory to complete the operation.
 * @return SRTP2_ERROR if an unspecified error occurs.
 */
SRTP2_Status_t SRTP2_msgListenRequest(SRTP2_MsgSAPI_t sapi, SRTP2_MsgIndicationCallback_t callback, SRTP2_CallbackContext_t callbackContext);

/** @} */

/** @} */
