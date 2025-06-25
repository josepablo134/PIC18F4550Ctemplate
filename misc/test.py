import serial
import time

def enter_reprogramming( timeout : float ) -> bool:
    t_a = time.time()
    with serial.Serial( "COM5", 115200, timeout=1/100_000 ) as s:
        s.flush()
        s.write(b'A')
        while( ( time.time() - t_a ) < timeout ):
            s.write(b'A')
            inmsg = s.read()
            if( len(inmsg) and inmsg == b'A' ):
                return True

    return False

# Try enter into reprogramming session for up to 10 seconds
if( enter_reprogramming( 10 ) ):
    print("reprogramming session active")
else:
    print("timeout reached")
