import time

CRC_INIT_VAL = 0xCE
COM_SOH=b'\x01'
COM_ACK=b'\x06'
COM_NACK=b'\x15'
COM_CAN=b'\x18'
COM_LF=b'\x0A'

def sendAck( s ):
    s.write( COM_ACK )

def sendNack( s ):
    s.write( COM_NACK )

def receive_byte( s , timeout:int = 60 ):
    t_a = time.time()
    single_byte = s.read()

    while( (len(single_byte) == 0) and ( ( time.time() - t_a ) < timeout ) ):
        single_byte = s.read()

    return single_byte
