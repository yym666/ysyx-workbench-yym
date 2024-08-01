ENTRY(_start)
PHDRS { bootloader1 PT_LOAD;
        bootloader2 PT_LOAD;
        text PT_LOAD; 
        data PT_LOAD; 
        }

MEMORY {
  flash : ORIGIN = 0x30000000, LENGTH = 32M
  sdram : ORIGIN = 0xa0000000, LENGTH = 32M
  sram : ORIGIN = 0x0f000000, LENGTH = 8K
}

SECTIONS {
    . = ORIGIN(flash);
    .bootloader1 : ALIGN(8){
        *(entry)
        *(.loader)
    } > flash AT > flash : bootloader1

    .bootloader2 : ALIGN(8){
        _ld2_start = .;
        *(.loader2)
        _ld2_end = .;
    } > sram AT > flash : bootloader2

    .text : ALIGN(8){
        _mytext = .;
        _text = .;
        *(.text*)
        _emytext = .;
    } > sdram AT > flash : text
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

    _data_cpy = LOADADDR(.data);
    _text_cpy = LOADADDR(.text);
    _loader2_cpy = LOADADDR(.bootloader2);

    . = _sram_start + SIZEOF(.bootloader2);
    _stack_top = ALIGN(0x100);
    . = _stack_top + 0x1000;
    _stack_pointer = .;
    end = .;
    _end = .;

    . = _bss_end + 0x20;
    _heap_start = ALIGN(0x100);
}