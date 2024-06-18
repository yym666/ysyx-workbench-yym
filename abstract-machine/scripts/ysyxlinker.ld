ENTRY(_start)
PHDRS { text PT_LOAD; data PT_LOAD; }

MEMORY {
  flash : ORIGIN = 0x30000000, LENGTH = 262144K
  sdram : ORIGIN = 0xa0000000, LENGTH = 32768K
  sram : ORIGIN = 0x0f000000, LENGTH = 8K
}

SECTIONS {
    . = ORIGIN(flash);
    .test : ALIGN(8){
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
    edata = .;
    _data = .;

    .bss : ALIGN(8){
        _bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(.scommon)
        _bss_end = .;
    } > sdram AT > flash

    . = _sdram_start + SIZEOF(.bss) + SIZEOF(.data);
    _stack_top = ALIGN(0x100);
    . = _stack_top + 0x800;
    _stack_pointer = .;
    end = .;
    _end = .;
    _heap_start = ALIGN(0x10);
}