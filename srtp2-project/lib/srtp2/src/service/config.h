#pragma once

enum {

    /**
     * Maximal number of sync subscriptions supported by the SRTP2 stack. Defaults to 16 subscriptions.
     */
#ifdef CONFIG_SRTP2_SYNC_SUBSCRIPTIONS_CAPACITY
    SYNC_SUBSCRIPTIONS_CAPACITY = CONFIG_SRTP2_SYNC_SUBSCRIPTIONS_CAPACITY,
#else
    SYNC_SUBSCRIPTIONS_CAPACITY = 16,
#endif

    /**
     * Maximal number of sampled values publish subscriptions supported by the SRTP2 stack. Defaults to 16 subscriptions.
     */
#ifdef CONFIG_SRTP2_SV_PUBLISH_SUBSCRIPTIONS_CAPACITY
    SV_PUBLISH_SUBSCRIPTIONS_CAPACITY = CONFIG_SRTP2_SV_PUBLISH_SUBSCRIPTIONS_CAPACITY,
#else
    SV_PUBLISH_SUBSCRIPTIONS_CAPACITY = 16,
#endif

    /**
     * Maximal length of sampled values payload supported by the SRTP2 stack. Defaults to 16 bytes.
     */
#ifdef CONFIG_SRTP2_SV_MAX_PAYLOAD_LENGTH
    SV_MAX_PAYLOAD_LENGTH = CONFIG_SRTP2_SV_MAX_PAYLOAD_LENGTH,
#else
    SV_MAX_PAYLOAD_LENGTH = 16,
#endif

    /**
     * Capacity of the event queue (maximal number of events between reception of beacons), defaults to 16.
     */
#ifdef CONFIG_SRTP2_EV_TX_QUEUE_CAPACITY
    EV_TX_QUEUE_CAPACITY = CONFIG_SRTP2_EV_TX_QUEUE_CAPACITY,
#else
    EV_TX_QUEUE_CAPACITY = 16,
#endif

    /**
     * Maximal length of events payload supported by the SRTP2 stack. Defaults to 16 bytes.
     */
#ifdef CONFIG_SRTP2_EV_MAX_PAYLOAD_LENGTH
    EV_MAX_PAYLOAD_LENGTH = CONFIG_SRTP2_EV_MAX_PAYLOAD_LENGTH,
#else
    EV_MAX_PAYLOAD_LENGTH = 16,
#endif

     /**
      * Maximal length of a message payload supported by the SRTP2 stack. Defaults to 512 bytes.
      */
#ifdef CONFIG_SRTP2_MSG_MAX_PAYLOAD_LENGTH
    MSG_MAX_PAYLOAD_LENGTH = CONFIG_SRTP2_MSG_MAX_PAYLOAD_LENGTH,
#else
    MSG_MAX_PAYLOAD_LENGTH = 512,
#endif

    /**
     * Capacity of the message transmit queue (maximal number of queued messages), defaults to 4.
     */
#ifdef CONFIG_SRTP2_MSG_TX_QUEUE_CAPACITY
    MSG_TX_QUEUE_CAPACITY = CONFIG_SRTP2_MSG_TX_QUEUE_CAPACITY,
#else
    MSG_TX_QUEUE_CAPACITY = 4,
#endif

    /**
     * Capacity of the message reception queue (maximal number of queued messages), defaults to 2.
     */
#ifdef CONFIG_SRTP2_MSG_RX_CAPACITY
    MSG_RX_CAPACITY = CONFIG_SRTP2_MSG_RX_CAPACITY,
#else
    MSG_RX_CAPACITY = 2,
#endif

   /**
    * Maximal number of message reception subscriptions supported by the SRTP2 stack. Defaults to 4 subscriptions.
    */
#ifdef CONFIG_SRTP2_MSG_LISTENERS_CAPACITY
    MSG_LISTENERS_CAPACITY = CONFIG_SRTP2_MSG_LISTENERS_CAPACITY,
#else
    MSG_LISTENERS_CAPACITY = 4,
#endif

    /**
     * Event flag used to inform the sync thread that a beacon has been received and the thread has to notify all subscribers about that.
     */
    SYNC_EVENT_FLAG_ID = 1 << 0,

    /**
     * Event flag used to inform the message thread that a message has been sent and the confirmation is in the confirmation queue.
     */
    MSG_CONFIRMATION_FLAG_ID = 1 << 1,

    /**
     * Event flag used to inform the message thread that a message has been received and the message payload and information is in the confirmation queue.
     */
    MSG_INDICATION_FLAG_ID = 1 << 2,
};
