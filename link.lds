/* Linker script for OR1200 program */

/* Linking for loading into external RAM */
MEMORY    
{
/*
	ld_info : ORIGIN = 0x00000000, LENGTH = 0x000000F0 
*/
/*	vectors : ORIGIN = 0x00100000, LENGTH = 0x2000 - 0x100
	ram : ORIGIN = 0x00102000, LENGTH = 0x00040000 - 0x00002000
*/
	vectors : ORIGIN = 0x00000000, LENGTH = 0x2000 - 0x100
	ram : ORIGIN = 0x00002000, LENGTH = 0x00200000 - 0x00002000
}

/*
  The following section defines where to put the different input sections.
  .text contains the code.
  .data contains the initialized data.
  .bss contains uninitialized data.
  .sdata contains small constant data.
*/

/* Stack information variables */
_min_stack      = 0x4000;   /* 16K - minimum stack space to reserve */

SECTIONS{	
/*
	.ld_info :{
      
	} > ld_info
*/

        .vectors :
        {
		*(.vectors)
        } > vectors

	.text : {
		_stext = .;
		*(.text)
		_etext  =  .;
		__CTOR_LIST__ = .;
		LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
		*(.ctors)
		LONG(0)
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
		*(.dtors)
		LONG(0)
		__DTOR_END__ = .;
		*(.lit)
		*(.shdata)
		_endtext = .;
	}  > ram

	.rodata : {
		*(.rodata);
		*(.rodata.*)
	} > ram
	
	.shbss :
	{
		*(.shbss)
	} > ram
	
	.talias :
	{ 
	}  > ram
	
	.data : {
		sdata  =  .;
		_sdata  =  .;
		*(.data)
		edata  =  .;
		_edata  =  .;
	} > ram

	.bss SIZEOF(.data) + ADDR(.data) :
	{
		sbss = . ;
		_sbss = . ;
		__bss_start = ALIGN(0x8);
		___bss_start = ALIGN(0x8);
		*(.bss)
		*(COMMON)
		end = ALIGN(0x8);
		_end = ALIGN(0x8);
		__end = ALIGN(0x8);
		ebss = .;
		_ebss = .;
	} > ram
	
	/* ensure there is enough room for stack */
        .stack (NOLOAD): {
                . = ALIGN(4);
		. = . + _min_stack ;
                . = ALIGN(4);
                stack = . ;
                _stack = . ;
        } > ram
        
        .stab  0 (NOLOAD) : {
		[ .stab ]
	}
	
	.stabstr  0 (NOLOAD) :{
		[ .stabstr ]
	}
}
