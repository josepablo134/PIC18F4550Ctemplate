import serial
import time

CRC_INIT_VAL = 0xCE
COMM_ACK=b'A'
COMM_NACK=b'N'
COMM_DATA=b'D'

APP_CFG_COMM_PAYLOAD_DATA_LEN       = (0x08)

APP_CFG_BLDR_CMD_ERASE              = (0xA0)
APP_CFG_BLDR_CMD_WRITE              = (0xA1)
APP_CFG_BLDR_CMD_READ               = (0xA2)
APP_CFG_BLDR_CMD_RESET              = (0xA3)

APP_CFG_COMTP_SERVICE_ID_DATA       = (0x01)
APP_CFG_COMTP_SERVICE_ID_SETUP      = (0x02)
APP_CFG_BOOTLDR_SERVICE_ID_CMD      = (0x03)

def sendAck( s ):
    s.write( COMM_ACK )

def sendNack( s ):
    s.write( COMM_NACK )

def enter_reprogramming( s, timeout : float ) -> bool:
    t_a = time.time()

    s.flush()
    sendAck( s )
    while( ( time.time() - t_a ) < timeout ):
        sendAck( s )
        inmsg = s.read()
        if( len(inmsg) and inmsg == COMM_ACK ):
            return True

    return False

def receive_byte( s ):
    single_byte = s.read()

    while( len(single_byte) == 0 ):
        single_byte = s.read()

    return single_byte

def receive_frame( s ) -> bytearray:
    frame = bytearray()
    single_byte = receive_byte( s )

    if( single_byte != COMM_DATA ):
        print( "invalid frame type : ", single_byte )
        return None

    crc_val = CRC_INIT_VAL
    single_byte = receive_byte( s )
    id_reg = int.from_bytes( single_byte, byteorder='little', signed=False )

    frame.append( id_reg )

    frame_len = (id_reg & 0x0F)
    if( frame_len > 8 ):
        print( "invalid frame len : ", frame_len )
        return None

    for byte_index in range( frame_len ):
        single_byte = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )
        frame.append( single_byte )
        crc_val ^= single_byte

    single_byte = int.from_bytes( receive_byte( s ), byteorder='little', signed=False )
    frame.append( single_byte )

    if( crc_val != single_byte ):
        print( "invalid crc val : ", crc_val )
        return None

    return frame

def send_frame( s , service_id : int, frame : bytearray ) -> bool:
    if( len(frame) > 8 ):
        return False

    if( service_id > 0x0F ):
        return False

    new_frame = bytearray( COMM_DATA )

    frame_id = (service_id & 0x0F) << 4
    frame_id |= len( frame ) & 0x0F

    new_frame.append( frame_id )

    crc_val = CRC_INIT_VAL
    for single_byte in frame:
        new_frame.append( single_byte )
        crc_val ^= single_byte

    new_frame.append( crc_val )

    s.write( new_frame )
    single_byte = receive_byte( s )

    # Check if frame was received!
    return ( single_byte == COMM_ACK )

def receive_package( s ) -> bytearray:
    package_bytes = bytearray()

    frame = receive_frame( s )
    if( frame == None ):
        return None

    frame_id = frame[ 0 ]
    frame_len = frame_id & 0x0F
    frame_id = (frame_id >> 4)
    if( (frame_id != APP_CFG_COMTP_SERVICE_ID_SETUP) or ( frame_len != 1 ) ):
        return None
    
    tp_byte_len = frame[ 1 ]

    if( tp_byte_len <= APP_CFG_COMM_PAYLOAD_DATA_LEN ):
        return None

    # SETUP FRAME RECEIVED

    frame_last_bytes = tp_byte_len % APP_CFG_COMM_PAYLOAD_DATA_LEN
    frames_count = int( (tp_byte_len - frame_last_bytes) / APP_CFG_COMM_PAYLOAD_DATA_LEN )

    for frame_index in range( frames_count ):
        # READ ALL FULL FRAMES
        frame = receive_frame( s )
        if( frame == None ):
            return None
        frame_id = frame[ 0 ]
        frame_len = frame_id & 0x0F
        frame_id = (frame_id >> 4)

        if( (frame_id != APP_CFG_COMTP_SERVICE_ID_DATA) or ( frame_len != APP_CFG_COMM_PAYLOAD_DATA_LEN ) ):
            return None

        # DATA FRAME RECEIVED
        # Append bytes from frame
        package_bytes += frame[1:9]

    if( frame_last_bytes != 0 ):
        # READ PARTIAL FRAME
        frame = receive_frame( s )
        if( frame == None ):
            return None

        frame_id = frame[ 0 ]
        frame_len = frame_id & 0x0F
        frame_id = (frame_id >> 4)

        if( (frame_id != APP_CFG_COMTP_SERVICE_ID_DATA) or ( frame_len != frame_last_bytes ) ):
            return None

        # DATA FRAME RECEIVED
        # Append bytes from frame
        package_bytes += frame[1: frame_last_bytes + 1 ]

    return package_bytes

def send_package( s, payload : bytearray ) -> bool:
    if( len(payload) != 32 ):
        return False

    tp_byte_len = len( payload )

    # Send 32 as the size of the TP package
    if( not send_frame( s, APP_CFG_COMTP_SERVICE_ID_SETUP, (32).to_bytes( 1, 'little' ) ) ):
        return False

    frame_last_bytes = tp_byte_len % APP_CFG_COMM_PAYLOAD_DATA_LEN
    frames_count = int( (tp_byte_len - frame_last_bytes) / APP_CFG_COMM_PAYLOAD_DATA_LEN )

    for frame_id in range( frames_count ):
        # Send Full Frames
        base_index = frame_id*APP_CFG_COMM_PAYLOAD_DATA_LEN
        last_index = base_index + APP_CFG_COMM_PAYLOAD_DATA_LEN
        frame = payload[ base_index : last_index ]
        if( not send_frame( s, APP_CFG_COMTP_SERVICE_ID_DATA, frame ) ):
            return False
    
    if( frame_last_bytes != 0 ):
        # Send partial Frame
        base_index = frames_count*APP_CFG_COMM_PAYLOAD_DATA_LEN
        last_index = base_index + frame_last_bytes
        frame = payload[ base_index : last_index ]
        if( not send_frame( s, APP_CFG_COMTP_SERVICE_ID_DATA, frame ) ):
            return False

    return True

def bootldr_cmd( s , cmd_id : int, addr : int = 0, size : int = 0 ) -> bool:
    if( size > 255 ):
        return False

    if( addr > 0xFFFF ):
        return False

    if( not ( cmd_id in [ APP_CFG_BLDR_CMD_ERASE, APP_CFG_BLDR_CMD_WRITE, APP_CFG_BLDR_CMD_READ, APP_CFG_BLDR_CMD_RESET ] ) ):
        return False
    
    frame_payload = bytearray()
    frame_payload.append( cmd_id )

    if( cmd_id != APP_CFG_BLDR_CMD_RESET ):
        # Only for READ/WRITE/EREASE
        frame_payload.append( (addr & 0xFF) )
        frame_payload.append( (addr >> 8) & 0xFF )
        frame_payload.append( size )

    return send_frame( s, service_id=APP_CFG_BOOTLDR_SERVICE_ID_CMD, frame=frame_payload )

def bootldr_com_flush( s ):
    byte_list = bytearray()
    s.write(b'\x00')
    single_byte = s.read()
    while( single_byte != b'' ):
        byte_list.append( int.from_bytes( single_byte ) )
        single_byte = s.read()
    return byte_list

def bootldr_erase( s, addr: int ) -> bool:
    bootldr_com_flush( s )
    return bootldr_cmd( s , APP_CFG_BLDR_CMD_ERASE, addr, 64 )

def bootldr_read( s, addr: int ) -> bytearray:
    bootldr_com_flush( s )
    if( not bootldr_cmd( s, APP_CFG_BLDR_CMD_READ, addr, 32 ) ):
        return None
    return receive_package( s )

def bootldr_write( s, addr: int, data : bytearray ) -> bool:
    bootldr_com_flush( s )
    if( not bootldr_cmd( s, APP_CFG_BLDR_CMD_WRITE, addr, 32) ):
        return False
    return send_package( s, data )

def bootldr_reset( s ) -> bool:
    bootldr_com_flush( s )
    return bootldr_cmd( s, APP_CFG_BLDR_CMD_RESET )
