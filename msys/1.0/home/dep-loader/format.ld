SECTIONS {
	.text 0x00100000: {
		*(.text)
		*(.text.*)
		QUAD(0)
		*(.gnu.linkonce.t*)
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		*(.rdata)
		*(.gcc_except_table)
	}

	.entry ALIGN(128): {
		*(.entry)
		*(.entry.*)
	}

	_gp = ALIGN(128) + 0x7ff0;

	/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) }

}