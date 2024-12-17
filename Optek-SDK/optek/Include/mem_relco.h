#ifndef __MEM_RELOC_H__
#define __MEM_RELOC_H__


#ifdef _WIN32

	// Empty defines for win32 platforms

#else
    #define __RESET_DSP_CORD_TEXT __attribute__ ((section (".reset_dsp_core_text")))

	#define __LIBKERNAL_RAM_TEXT __attribute__ ((section (".libkernel_text")))

	#define __FLASH_BOOT_TEXT   __attribute__ ((section (".flash_boot_text")))

	#define __INTERNAL_RAM_TEXT __attribute__ ((section (".internal_ram_1_text")))

	#define __INTENAL_RAM_TEXT  __attribute__ ((section (".interal_ram_1_text")))

// For placing specific variables into iphone_bss section

	#define __IPHONE_BSS        __attribute__ ((section (".iphone_bss")))

	#define __USBHOST_BSS       __attribute__ ((section (".usbhost_bss")))
	#define __USBHOST_TEXT      __attribute__ ((section (".internal_ram_1_text")))

	#define __FS_SEARCH_FILE_TEXT __attribute__ ((section (".fs_search_file_text")))

	#define __USB_DEV_BSS       __attribute__ ((section (".usbDev_bss")))
	#define __US_DEV_TEXT       __attribute__ ((section (".internal_ram_1_text")))

	#define __MTPHOST_BSS       __attribute__ ((section (".mtphost_bss")))


	#define __CD_BSS            __attribute__ ((section (".cd_bss")))


	#define __USB_FS_BSS        __attribute__ ((section (".usb_fs_bss")))

	#define __USB_FOLDER_BSS     __attribute__ ((section (".usb_folder_bss")))

	#define __USB_FS_DELETE_BSS   __attribute__ ((section (".usb_fs_delete_bss")))

	#define __UPGRADE_HW_SET_RODATA    __attribute__ ((section (".upgrade_hw_set_rodata")))

	#define __EXTERNAL_BSS        __attribute__ ((section (".external_bss")))
	#define __EXTERNAL_TEXT       __attribute__ ((section (".external_text")))
#endif

	#define __UPGRADE_HW_SET_RODATA    __attribute__ ((section (".upgrade_hw_set_rodata")))

	#define __APP_UPGRADE_OK_CHECK_RODATA    __attribute__ ((section (".app_upgrade_ok_check_rodata")))

	#define __PRINTF_BSS        __attribute__ ((section (".printf_bss")))

	#define __PROMPT_MUSIC_RODATA      //__attribute__ ((section (".pormpt_music_rodata")))
	
#endif //__MEM_RELOC_H__
