from .common import COM_SOH,COM_NACK,COM_ACK,CRC_INIT_VAL
from .common import receive_byte,sendAck,sendNack

XSMODEM_CFG_MAX_BUFFER_SIZE       = (128)
XSMODEM_CFG_TIMEOUT_MS            = (100)
XSMODEM_CFG_RETRIES_COUNT         = (10)
XSMODEM_CFG_CRC8_INIT_VAL         = (0xCE)

def transmit( s , frame : bytearray, timeout : float = 60 ) -> bool:
    if( len(frame) > XSMODEM_CFG_MAX_BUFFER_SIZE ):
        return False

    new_frame = bytearray( COM_SOH )

    new_frame.append( len( frame ) )
    new_frame.append( 0xFF & ( ~len( frame ) ) )

    crc_val = CRC_INIT_VAL
    for single_byte in frame:
        new_frame.append( single_byte )
        crc_val ^= single_byte

    new_frame.append( crc_val )

    single_byte = receive_byte( s , timeout )

    if( single_byte != COM_NACK ):
        return False

    # for single_byte in new_frame:
    #     s.write( single_byte.to_bytes( 1 , 'little' ) )
    #     confirmation = receive_byte( s , timeout )
    #     print( hex(single_byte), "=>", hex( int.from_bytes( confirmation ) ) )
    #     # if( confirmation != COM_ACK ):
    #     #     return False
    # print("")

    s.write( new_frame )

    single_byte = receive_byte( s , timeout )
    # Confirm packet reception
    return ( single_byte == COM_ACK )

def receive( s, timeout : float = 60  ) -> bytearray:
    crc_val = CRC_INIT_VAL
    frame = bytearray()

    # Send ready signal
    sendNack( s )

    single_byte = receive_byte( s , timeout )
    if( single_byte != COM_SOH ):
        sendNack( s )
        return None

    packet_len = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )
    packet_len_n = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )

    if( (packet_len != (~packet_len_n & 0xFF)) or (packet_len > XSMODEM_CFG_MAX_BUFFER_SIZE) ):
        sendNack( s )
        return None

    for packet_id in range( packet_len ):
        single_byte = int.from_bytes( receive_byte( s , timeout ), byteorder='little', signed=False )
        frame.append( single_byte )
        crc_val ^= single_byte

    single_byte = int.from_bytes( receive_byte( s , timeout ), byteorder='little', signed=False )

    if( crc_val != single_byte ):
        sendNack( s )
        return None

    sendAck( s )
    return frame
