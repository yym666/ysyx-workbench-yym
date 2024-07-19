ENTRY(_start)
PHDRS { text PT_LOAD; 
        data PT_LOAD; 
        }

MEMORY {
  flash : ORIGIN = 0x30000000, LENGTH = 16M
  sdram : ORIGIN = 0xa0000000, LENGTH = 8M
  sram : ORIGIN = 0x0f000000, LENGTH = 8K
}

SECTIONS {
    . = ORIGIN(flash);
    .text : ALIGN(8){
        *(entry)
        *(.text*)
    } > flash AT > flash : text
    etext = .;
    _etext = .;

    .rodata : ALIGN(8){
        _rodata = .;
        *(.rodata*)
        *(.srodata*)
        _erodata = .;
    } > flash AT > flash

    .data : ALIGN(8){
        _data = .;
        *(.data*)
        *(.sdata*)
        _edata = .;
    } > sdram AT > flash : data

    .bss : ALIGN(8){
        _bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(.scommon)
        _bss_end = .;
    } > sdram AT > flash

    . = _sram_start;
    _stack_top = ALIGN(0x400);
    . = _stack_top + 0x1800;
    _cpy_begin = LOADADDR(.data);
    _stack_pointer = .;
    end = .;
    _end = .;

    . = _bss_end + 0x20;
    _heap_start = ALIGN(0x400);
}