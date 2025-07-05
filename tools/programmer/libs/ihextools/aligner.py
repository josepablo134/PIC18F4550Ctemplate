
from intelhex import IntelHex

def align_segment( base_addr, size, alignment ) -> tuple[int,int]:
    if( base_addr % alignment ):
        base_addr = base_addr - (base_addr % alignment)

    if( size % alignment ):
        size = size + (alignment - (size % alignment) )

    return (base_addr, size)

def align_hex( ih : IntelHex, alignment : int ) -> IntelHex:
    aligned_ih = IntelHex()

    for segment in ih.segments():
        base_addr = segment[0]
        last_addr = segment[1]
        size = last_addr - base_addr

        aligned_base_address,aligned_size = align_segment( base_addr, size, alignment )
        aligned_last_address = aligned_base_address + aligned_size

        for addr in range(aligned_base_address,aligned_last_address):
            aligned_ih[ addr ] = ih[ addr ]

    return aligned_ih
