#ifndef _XMODEM_H_
#define _XMODEM_H_

	#include "xModem_cfg.h"
	#include "xModem_types.h"
	#include "xModem_include.h"

	/**
	 * Block until the packet is sent and confirmed by the client
	 */
	extern xmodem_std_status_t xModem_Send( xmodem_packet_t* packet, xmodem_bool_t timeout_enabled );

	/**
	 * Block until the packet is received or an error happens
	 */
	extern xmodem_std_status_t xModem_Receive( xmodem_packet_t* packet, xmodem_bool_t timeout_enabled );

	/**
	 * Wait for an ACK signal
	 */
	extern xmodem_std_status_t xModem_waitAck( xmodem_bool_t timeout_enabled );

	/**
	 * Send an ACK signal
	 */
	extern void xModem_Ack( void );

	/**
	 * Send an NACK signal
	 */
	extern void xModem_Nack( void );

#endif
