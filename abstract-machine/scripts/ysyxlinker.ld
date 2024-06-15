ENTRY(_start)
PHDRS { text PT_LOAD; data PT_LOAD; }

MEMORY {
  flash : ORIGIN = 0x30000000, LENGTH = 262144K
  sram : ORIGIN = 0x0f000000, LENGTH = 8K
}


SECTIONS {
    . = ORIGIN(flash);
    .test : {
        *(entry)
        *(.text*)
    } > flash AT > flash : text
    etext = .;
    _etext = .;

    .rodata : {
        _rodata = .;
        *(.rodata*)
        *(.srodata*)
        _erodata = .;
    } > flash AT > flash

    .data : {
        _data = .;
        *(.data*)
        *(.sdata*)
        _edata = .;
    } > sram AT > flash : data
    edata = .;
    _data = .;

    .bss : {
        _bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(.scommon)
        _bss_end = .;
    } > sram AT > flash

    . = _sram_start + SIZEOF(.bss) + SIZEOF(.data);
    _stack_top = ALIGN(0x100);
    . = _stack_top + 0x800;
    _stack_pointer = .;
    end = .;
    _end = .;
    _heap_start = ALIGN(0x10);
}