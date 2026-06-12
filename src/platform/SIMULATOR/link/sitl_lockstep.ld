/* SITL_LOCKSTEP linker script.
 *
 * Same .pg_registry placement as sitl.ld, plus bounds symbols around the
 * firmware's entire mutable image (.pg_registry .. .bss). The per-instance
 * state machinery copies [__bf_inst_start, __bf_inst_end) per firmware
 * instance and rebases pointers inside it using the relocation records
 * kept in the binary by --emit-relocs.
 */

SECTIONS {
  /* BLOCK: on Windows (PE) output section must be page-aligned. Use 4-byte alignment otherwise */
  /* SUBALIGN: force 4-byte alignment of input sections for pg_registry.
     Gcc defaults to 32 bytes; padding is then inserted between object files, breaking the init structure. */
  .pg_registry BLOCK( DEFINED(__section_alignment__) ? __section_alignment__ : 4 ) :   SUBALIGN(4)
  {
    PROVIDE_HIDDEN (__bf_inst_start = . );
    PROVIDE_HIDDEN (__pg_registry_start = . );
    PROVIDE_HIDDEN (___pg_registry_start = . );
    KEEP (*(.pg_registry))
    KEEP (*(SORT(.pg_registry.*)))
    PROVIDE_HIDDEN (__pg_registry_end = . );
    PROVIDE_HIDDEN (___pg_registry_end = . );

    PROVIDE_HIDDEN (__pg_resetdata_start = . );
    PROVIDE_HIDDEN (___pg_resetdata_start = . );
    KEEP (*(.pg_resetdata))
    PROVIDE_HIDDEN (__pg_resetdata_end = . );
    PROVIDE_HIDDEN (___pg_resetdata_end = . );
  }
}
INSERT BEFORE .fini_array;

SECTIONS {
  .bf_inst_end (NOLOAD) : {
    PROVIDE_HIDDEN (__bf_inst_end = . );
  }
}
INSERT AFTER .bss;
