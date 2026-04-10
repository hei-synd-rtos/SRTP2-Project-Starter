#include <srtp2.h>
#include "blinky.h"
#include "time.h"
#include "nunchuk.h"
#include "chat.h"
#include <zephyr/net/net_if.h>

int main(void) {
	// Initialize the SRTP2 protocol stack.
	SRTP2_init(net_if_get_default());

	// Initialize all applications.
	Blinky_init();
    Time_init();
    Nunchuk_init();
	Chat_init();
	return 0;
}
