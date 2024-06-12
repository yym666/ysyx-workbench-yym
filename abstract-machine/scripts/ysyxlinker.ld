ENTRY(_start)
PHDRS { text PT_LOAD; data PT_LOAD; }

MEMORY {
  mrom : ORIGIN = 0x20000000, LENGTH = 4K
  sram : ORIGIN = 0x0f000000, LENGTH = 8K
}


SECTIONS {
    . = ORIGIN(mrom);
    .test : {
        *(entry)
        *(.text*)
    } > mrom AT > mrom : text
    etext = .;
    _etext = .;

    .rodata : {
        *(.rodata*)
    } > mrom AT > mrom

    . = _sram_start;
    .data : {
        *(.data)
    } > sram AT > mrom : data
    edata = .;
    _data = .;

    .bss : {
        _bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(.scommon)
    } > sram AT > mrom

    _stack_top = ALIGN(0x100);
    . = _stack_top + 0x800;
    _stack_pointer = .;
    end = .;
    _end = .;
    _heap_start = ALIGN(0x10);
}