import argparse
import sys
import json
from intelhex import IntelHex
from libs.devices.pic18f4550 import program_device

def align_segment( base_addr, size, alignment ) -> tuple[int,int]:
    if( base_addr % alignment ):
        base_addr = base_addr - (base_addr % alignment)

    if( size % alignment ):
        size = size + (alignment - (size % alignment) )

    return (base_addr, size)

def getArgParser():
    parser = argparse.ArgumentParser(description='MemMap Headers Generator',exit_on_error=True)
    parser.add_argument("-i","--input", nargs=1, required=True, help="Input File Path (hex file format)" , dest="input_file_path")
    parser.add_argument("-p","--port", nargs=1, required=True, help="Serial Port", dest="serial_port")
    parser.add_argument("-d","--device-configuration", nargs=1, required=True, help="Device configuration (json file)", dest="device_configuration")
    return parser

def main():
    args = getArgParser().parse_args( sys.argv[1:] )
    input_file_path = args.input_file_path[0]
    serial_port = args.serial_port[0]
    device_configuration_file_path = args.device_configuration[0]

    ih = IntelHex( input_file_path )

    with open( device_configuration_file_path , "r" ) as device_config_file:
        device_config = json.load( device_config_file )

    program_device( serial_port, ih, device_config )

if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print(error, file=sys.stderr)
