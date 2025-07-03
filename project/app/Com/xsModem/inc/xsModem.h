#ifndef _XSMODEM_H_
#define _XSMODEM_H_

	#include "xsModem_cfg.h"
	#include "xsModem_types.h"
	#include "xsModem_include.h"

	/**
	 * Block until the packet is sent and confirmed by the client
	 */
	extern xsmodem_std_status_t xsModem_Transmit( xsmodem_packet_t* packet, xsmodem_bool_t timeout_enabled );

	/**
	 * Block until the packet is received or an error happens
	 */
	extern xsmodem_std_status_t xsModem_Receive( xsmodem_packet_t* packet, xsmodem_bool_t timeout_enabled );

	/**
	 * Wait for an ACK signal
	 */
	extern xsmodem_std_status_t xsModem_waitAck( xsmodem_bool_t timeout_enabled );

	/**
	 * Wait for an NAK signal
	 */
	extern xsmodem_std_status_t xsModem_waitNack( xsmodem_bool_t timeout_enabled );

	/**
	 * Send an ACK signal
	 */
	extern void xsModem_Ack( void );

	/**
	 * Send an NACK signal
	 */
	extern void xsModem_Nack( void );

#endif
