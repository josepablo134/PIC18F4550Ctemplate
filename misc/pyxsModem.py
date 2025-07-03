import serial
import time

CRC_INIT_VAL = 0xCE
COM_SOH=b'\x01'
COM_ACK=b'\x06'
COM_NACK=b'\x15'
COM_CAN=b'\x18'
COM_LF=b'\x0A'

XSMODEM_CFG_MAX_BUFFER_SIZE       = (128)
XSMODEM_CFG_TIMEOUT_MS            = (100)
XSMODEM_CFG_RETRIES_COUNT         = (10)
XSMODEM_CFG_CRC8_INIT_VAL         = (0xCE)

def print_byte_as_hex( packet : bytearray ):
    for single_byte in packet:
        print( f"{hex(single_byte)} " , end="" )
    print("")

def sendAck( s ):
    s.write( COM_ACK )

def sendNack( s ):
    s.write( COM_NACK )

def enter_reprogramming( s, timeout : float ) -> bool:
    t_a = time.time()

    s.flush()
    sendAck( s )
    while( ( time.time() - t_a ) < timeout ):
        sendAck( s )
        inmsg = s.read()
        if( len(inmsg) and inmsg == COM_ACK ):
            return True

    return False

def receive_byte( s ):
    single_byte = s.read()

    while( len(single_byte) == 0 ):
        single_byte = s.read()

    return single_byte

def transmit( s , frame : bytearray ) -> bool:
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

    single_byte = receive_byte( s )

    if( single_byte != COM_NACK ):
        return False

    s.write( new_frame )
    single_byte = receive_byte( s )

    # Confirm packet reception
    return ( single_byte == COM_ACK )

def receive( s ) -> bytearray:
    crc_val = CRC_INIT_VAL
    frame = bytearray()

    # Send ready signal
    sendNack( s )

    single_byte = receive_byte( s )
    if( single_byte != COM_SOH ):
        sendNack( s )
        return None

    packet_len = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )
    packet_len_n = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )

    if( (packet_len != (~packet_len_n & 0xFF)) or (packet_len > XSMODEM_CFG_MAX_BUFFER_SIZE) ):
        sendNack( s )
        return None

    for packet_id in range( packet_len ):
        single_byte = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )
        frame.append( single_byte )
        crc_val ^= single_byte

    single_byte = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )

    if( crc_val != single_byte ):
        sendNack( s )
        return None

    sendAck( s )
    return frame

def read_block( s, addr : int, len : int ) -> bytearray:
    addr = addr & 0xFFFF
    addr_n = (~addr) & 0xFFFF
    len = len & 0xFF
    len_n = (~len) & 0xFF

    # READ Memory CMD char
    cmd_bytes = bytearray(b'R')

    cmd_bytes.append( addr & 0xFF )
    cmd_bytes.append( (addr>>8) & 0xFF )

    cmd_bytes.append( addr_n & 0xFF )
    cmd_bytes.append( (addr_n>>8) & 0xFF )

    cmd_bytes.append( len )
    cmd_bytes.append( len_n )

    s.write( cmd_bytes )
    single_byte = receive_byte( s )

    if( single_byte != COM_ACK ):
        return None
    return receive( s )

def write_block( s, addr: int, block: bytearray ) -> bool:
    addr = addr & 0xFFFF
    addr_n = (~addr) & 0xFFFF

    if( len( block ) > XSMODEM_CFG_MAX_BUFFER_SIZE ):
        return False

    # PROGRAM Memory CMD char
    cmd_bytes = bytearray(b'P')

    cmd_bytes.append( addr & 0xFF )
    cmd_bytes.append( (addr>>8) & 0xFF )

    cmd_bytes.append( addr_n & 0xFF )
    cmd_bytes.append( (addr_n>>8) & 0xFF )

    s.write( cmd_bytes )
    single_byte = receive_byte( s )

    if( single_byte != COM_ACK ):
        return False

    if( not transmit( s , block ) ):
        return False

    # Program process confirmed by device
    single_byte = receive_byte( s )

    return ( single_byte == COM_ACK )


def reset_device( s ) -> bool:
    s.write( COM_LF )
    single_byte = receive_byte( s )

    if( single_byte != COM_ACK ):
        return False

    return True
