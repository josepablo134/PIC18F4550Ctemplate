import serial
import time
from intelhex import IntelHex
from ..ihextools.aligner import align_hex
from ..com.bootloader import enter_reprogramming,write_block,reset_device,ping,read_block

class PIC18F45450Error( Exception ):
    pass

CNF_REPROGRAMMING_TIMEOUT=10
CNF_CMD_TIMEOUT=1

def get_blocks( ihex: IntelHex, sector_size: int, block_size: int ) -> list[ tuple[int, bytearray] ]:
    block_list = []

    ih = align_hex( ihex, sector_size )

    for base_addr,last_addr in ih.segments():
        if( base_addr == 0x300000 ):
            # Ignore configuration bits
            continue
        if( base_addr == 0x200000 ):
            # Ignore id bits
            continue
        content_len = last_addr - base_addr

        if( content_len <= block_size ):
            content = bytearray()
            for addr in range(base_addr, last_addr):
                content.append( ih[ addr ] )
            block_list.append( [ base_addr, content ] )
        else:
            num_of_blocks = int(content_len / block_size)
            for block_id in range( num_of_blocks ):
                block_base_addr = base_addr + (block_size * block_id)
                block_last_addr = block_base_addr + block_size
                block_conent = bytearray()

                for addr in range(block_base_addr, block_last_addr):
                    block_conent.append( ih[ addr ] )

                block_list.append( [ block_base_addr, block_conent ] )

    return block_list


def print_byte_as_hex( packet : bytearray ) -> None:
    if( len(packet) == 0 ):
        return

    for single_byte in packet:
        print( f"{hex(single_byte)} " , end="" )
    print("")

def flush_ser_port( ser_port ) ->None :
    data = bytearray()
    single_byte = ser_port.read()
    while( single_byte != b'' ):
        data.append( int.from_bytes( single_byte ) )
        single_byte = ser_port.read()
    print_byte_as_hex( data )

def program_device( ser_port:str, ihex: IntelHex, dev_config : dict[str,str] ):
    baudrate = dev_config.get("baudrate")
    flash_offset = dev_config.get("flash_offset")
    sector_size = dev_config.get("sector_size")
    block_size = dev_config.get("block_size")
    page_size = dev_config.get("page_size")

    if( isinstance( flash_offset, str ) ):
        flash_offset = int( flash_offset, 16 )

    if( not baudrate ):
        raise PIC18F45450Error("invalid baudrate")

    if( not flash_offset ):
        raise PIC18F45450Error("invalid reset vector")

    if( not sector_size ):
        raise PIC18F45450Error("invalid sector size")

    if( not page_size ):
        raise PIC18F45450Error("invalid page size")

    if( not block_size ):
        raise PIC18F45450Error("invalid block size")
    
    if( (block_size % sector_size) > 0 ):
        raise PIC18F45450Error(f"block size of {block_size} is not aligned with sector of {sector_size}")

    # Assuming page < sector which is the expected scenario
    if( (sector_size % page_size) > 0 ):
        raise PIC18F45450Error(f"sector size of {sector_size} is not aligned with page of {page_size}")

    s = serial.Serial( ser_port, baudrate, timeout=1/100_000 )

    print("Serial port connected ...")

    if( not ping( s, CNF_CMD_TIMEOUT) ):
        if( not enter_reprogramming( s , CNF_REPROGRAMMING_TIMEOUT ) ):
            raise PIC18F45450Error("Reprogramming mode not detected")

    print("Reprogramming mode enabled ...")

    flush_ser_port( s )

    for i in range(10):
        if( not ping( s, CNF_CMD_TIMEOUT ) ):
            raise PIC18F45450Error("Device not responding")

    print("Device is online...")

    flush_ser_port( s )

    block_id = 0
    for addr,content in get_blocks( ihex, sector_size, block_size ):
        print(f"Writing block {block_id} @ [ {hex(addr)} : {hex(addr+len(content))} ]")

        # Translate from hardware address to flash driver virtual address
        addr = addr - flash_offset
        if( not write_block( s, addr, content, CNF_CMD_TIMEOUT ) ):
            print( addr , len(content) )
            flush_ser_port( s )
            raise PIC18F45450Error(f"Error when writing block {block_id}")
        block_id += 1

    print("Reprogramming completed!")
    
    if( not reset_device( s ) ):
        raise PIC18F45450Error("Device not reset")

    print("Application running ...")
