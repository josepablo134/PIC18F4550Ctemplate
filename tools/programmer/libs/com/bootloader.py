import time

from .common import COM_ACK,COM_LF
from .common import sendAck,receive_byte

from .xsmodem import XSMODEM_CFG_MAX_BUFFER_SIZE
from .xsmodem import transmit,receive

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

def ping( s , timeout : float = 60  ) -> bool:
    s.write( b'A' )
    b = receive_byte( s , timeout )
    return ( b == b'A' )

def read_block( s, addr : int, len : int, timeout : float = 60  ) -> bytearray:
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
    single_byte = receive_byte( s , timeout )

    if( single_byte != COM_ACK ):
        return None
    return receive( s )

def write_block( s, addr: int, block: bytearray, timeout : float = 60 ) -> bool:
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
    single_byte = receive_byte( s , timeout )

    if( single_byte != COM_ACK ):
        return False

    if( not transmit( s, block, timeout ) ):
        return False

    # Program process confirmed by device
    single_byte = receive_byte( s , timeout )
    return ( single_byte == COM_ACK )

def reset_device( s, timeout : float = 60 ) -> bool:
    s.write( COM_LF )
    single_byte = receive_byte( s , timeout )

    if( single_byte != COM_ACK ):
        return False

    return True
