#pragma once

/**
 * @brief Initializes the Chat module for the application.
 *
 * This function sets up the Chat module by performing the following steps:
 * - Checks if the GPIO associated with the external LED (ld3) is ready for use.
 * - Configures the GPIO pin as an active output and initializes it to a low state.
 * - Registers a callback function `Chat_messageIndication` to handle incoming SRTP2 messages on the specified SAPI.
 * - Registers a shell command named "chat" that allows users to send a chat message over the SRTP2 protocol. The command must be invoked with specific
 *   arguments.
 *
 * If any initialization step fails (e.g., GPIO is not ready, message listener registration  fails), the corresponding error is logged, and initialization
 * stops.
 *
 * To send a message, you can use the `chat` shell command. The syntax is `char <mac> <"Text message">`. For example you can send the message "Hello World!" to
 * all nodes on the network using `char ff:ff:ff:ff:ff:ff "Hello World!"`.
 *
 * Additionally, the chat application can be used to control LD4 on the HEI Discovery board. Sending the command `led on` will turn the LED on, sending
 * `led off` will turn the LED off and sending `led toggle` will toggle the LED state.
 */
void Chat_init();
