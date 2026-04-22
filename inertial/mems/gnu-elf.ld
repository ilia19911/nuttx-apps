SECTIONS
{
  .text 0x0 :
    {
      _stext = . ;
      *(.text)
      *(.text.*)
      *(.gnu.warning)
      *(.stub)
      *(.glue_7)
      *(.glue_7t)
      *(.jcr)
      . = ALIGN(4);
      _etext = . ;
    }
  .rodata :
    {
      _srodata = . ;
      *(.rodata)
      *(.rodata1)
      *(.rodata.*)
      *(.gnu.linkonce.r*)
      . = ALIGN(4);
      _erodata = . ;
    }
  .data :
    {
      _sdata = . ;
      *(.data)
      *(.data1)
      *(.data.*)
      *(.gnu.linkonce.d*)
      . = ALIGN(4);
      _edata = . ;
    }
  .init_array :
    {
      _sinit = .;
      _sctors = .;
      KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
      KEEP(*(.init_array .ctors))
      . = ALIGN(4);
      _einit = .;
      _ectors = .;
    }
  .fini_array :
    {
      _sfini = .;
      _sdtors = .;
      KEEP (*(.dtors))
      KEEP (*(.fini_array))
      KEEP (*(SORT(.fini_array.*)))
      . = ALIGN(4);
      _efini = .;
      _edtors = .;
    }
  .bss :
    {
      _sbss = . ;
      *(.bss)
      *(.bss.*)
      *(.sbss)
      *(.sbss.*)
      *(.gnu.linkonce.b*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = . ;
    }
  .got :
    {
      _sgot = .;
      _GLOBAL_OFFSET_TABLE_ = .;
      *(.got)
      *(.got.plt)
      *(.igot.plt)
      *(.got.*)
      . = ALIGN(4);
      _egot = .;
    }
    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_info 0 : { *(.debug_info) }
    .debug_line 0 : { *(.debug_line) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_aranges 0 : { *(.debug_aranges) }
}
