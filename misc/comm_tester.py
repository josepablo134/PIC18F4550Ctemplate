import serial

def connect():
    return serial.Serial("COM5", 115200, timeout=1)

def read_frame( s ):
    not_valid = True

    single_byte = s.read()

    while( not_valid ):
        while( single_byte != b'D' ):
            single_byte = s.read() # Look for Frame header
        single_byte = s.read() # Look for the frame id

        id = int.from_bytes( single_byte )
        frame_len = (id & 0x0F)
        if( frame_len > 8 ):
            # Skip frame for invalid format
            # Try to use this byte as Frame header in  the next iteration
            continue
        else:
            not_valid = False

    data_buffer = s.read( frame_len + 1 )

    return id,data_buffer
