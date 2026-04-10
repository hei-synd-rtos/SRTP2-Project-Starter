#include "srtp2/types.h"
#include <srtp2.h>
#include <cmsis_os2.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(srtp2, LOG_LEVEL_INF);

// Thread functions forward declarations.
void SRTP2_syncThread_(void*);
void SRTP2_networkThread_(void*);
void SRTP2_msgThread_(void*);

SRTP2_NetworkDevice_t networkDevice_;
int networkSocket_;

SRTP2_Status_t SRTP2_init(SRTP2_NetworkDevice_t networkDevice) {
    // Setup networking.
    networkDevice_ = networkDevice;
    networkSocket_ = zsock_socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (networkSocket_ < 0) {
        LOG_ERR("socket creation failed: %d (errno: %d - %s)\n", networkSocket_, errno, strerror(errno));
        return SRTP2_ERROR;
    }

    struct sockaddr_ll addr = {0};
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = net_if_get_by_iface(networkDevice);
    addr.sll_protocol = htons(0xacdc);
    if (zsock_bind(networkSocket_, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
        LOG_ERR("failed to bind layer 2 socket: (errno: %d - %s)", errno, strerror(errno));
        return SRTP2_ERROR;
    }

    // TODO: Initialize SRTP2 services and networking.

    return SRTP2_ERROR_NOT_IMPLEMENTED;
}
