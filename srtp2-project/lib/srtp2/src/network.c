#include <srtp2.h>
#include "protocol/frame.h"
#include "protocol/mpdu.h"
#include "protocol/offsets.h"
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2_net, LOG_LEVEL_INF);

extern SRTP2_NetworkDevice_t networkDevice_;
extern int networkSocket_;

static uint8_t rxFrame_[SRTP2_FRAME_MTU];   // Frame buffer for RX.
static uint8_t txFrame_[SRTP2_FRAME_MTU];   // Frame buffer for TX.
static struct sockaddr_ll txAddress_ = {AF_PACKET, 0, 0, 0, 0, 6};

void SRTP2_networkThread_(void*) {
    LOG_DBG("starting networking thread");

    // Set all bytes to zero in the RX and TX buffers.
    memset(rxFrame_, 0x00, SRTP2_FRAME_MTU);
    memset(txFrame_, 0x00, SRTP2_FRAME_MTU);

    // Copy own MAC address to the TX frame.
    struct net_linkaddr* linkAddress = net_if_get_link_addr(networkDevice_);
    if (linkAddress != NULL) {
        memcpy(txFrame_ + FRAME_SOURCE_ADDRESS_OFFSET, linkAddress->addr, FRAME_SOURCE_ADDRESS_LENGTH);
    } else {
        LOG_ERR("failed to get ethernet MAC address");
    }

    // Prepare TX address.
    txAddress_.sll_ifindex = net_if_get_by_iface(networkDevice_);

    while (true) {
        // Receive a frame.
        ssize_t rxFrameLength = zsock_recv(networkSocket_, rxFrame_, SRTP2_FRAME_MTU, 0);
        if (rxFrameLength < 0) {
            LOG_ERR("failed to receive Ethernet frame");
            continue;
        }

        // TODO: Handle network frames.

        // Example: Send an empty MPDU frame back.
        SRTP2_MPDU_Builder_t builder;
        SRTP2_MPDU_Builder_init(&builder, txFrame_, SRTP2_FRAME_MTU);
        ssize_t status = zsock_sendto(networkSocket_, txFrame_, SRTP2_MPDU_Builder_length(&builder), 0, (struct sockaddr*)(&txAddress_), sizeof(txAddress_));
        if (status < 0) {
            LOG_ERR("failed to send MPDU frame");
        }
    }
}
