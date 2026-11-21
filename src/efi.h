#pragma once
#include <stdint.h>
#include <stddef.h>
#define EFIAPI __attribute__((ms_abi))
typedef uint64_t EFI_STATUS;
typedef void* EFI_HANDLE;
typedef uint16_t CHAR16;
typedef struct { uint32_t a; uint16_t b,c; uint8_t d[8]; } EFI_GUID;
typedef struct { uint64_t signature; uint32_t revision, header_size, crc32, reserved; } EFI_TABLE_HEADER;
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL { void *reset; EFI_STATUS (EFIAPI *output_string)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, const CHAR16*); void *rest[8]; };
typedef struct EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;
struct EFI_FILE_PROTOCOL {
 uint64_t revision;
 EFI_STATUS (EFIAPI *open)(EFI_FILE_PROTOCOL*, EFI_FILE_PROTOCOL**, CHAR16*, uint64_t, uint64_t);
 EFI_STATUS (EFIAPI *close)(EFI_FILE_PROTOCOL*);
 void *delete_file;
 EFI_STATUS (EFIAPI *read)(EFI_FILE_PROTOCOL*, size_t*, void*);
 void *write,*get_position,*set_position,*get_info,*set_info,*flush;
};
typedef struct { uint64_t revision; EFI_STATUS (EFIAPI *open_volume)(void*, EFI_FILE_PROTOCOL**); } EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
typedef struct {
 uint32_t type; uint32_t pad; uint64_t physical_start, virtual_start, number_of_pages, attribute;
} EFI_MEMORY_DESCRIPTOR;
typedef struct EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
struct EFI_BOOT_SERVICES {
 EFI_TABLE_HEADER hdr;
 void *raise_tpl,*restore_tpl;
 EFI_STATUS (EFIAPI *allocate_pages)(uint32_t,uint32_t,size_t,uint64_t*);
 EFI_STATUS (EFIAPI *free_pages)(uint64_t,size_t);
 EFI_STATUS (EFIAPI *get_memory_map)(size_t*,EFI_MEMORY_DESCRIPTOR*,size_t*,size_t*,uint32_t*);
 EFI_STATUS (EFIAPI *allocate_pool)(uint32_t,size_t,void**);
 EFI_STATUS (EFIAPI *free_pool)(void*);
 void *create_event,*set_timer,*wait_for_event,*signal_event,*close_event,*check_event;
 void *install_protocol_interface,*reinstall_protocol_interface,*uninstall_protocol_interface;
 EFI_STATUS (EFIAPI *handle_protocol)(EFI_HANDLE,EFI_GUID*,void**);
 void *reserved,*register_protocol_notify,*locate_handle,*locate_device_path,*install_configuration_table;
 void *load_image,*start_image,*exit,*unload_image;
 EFI_STATUS (EFIAPI *exit_boot_services)(EFI_HANDLE,size_t);
};
typedef struct {
 EFI_TABLE_HEADER hdr;
 CHAR16 *firmware_vendor; uint32_t firmware_revision; uint32_t pad;
 EFI_HANDLE console_in_handle; void *con_in;
 EFI_HANDLE console_out_handle; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *con_out;
 EFI_HANDLE standard_error_handle; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *std_err;
 void *runtime_services; EFI_BOOT_SERVICES *boot_services;
 size_t number_of_table_entries; void *configuration_table;
} EFI_SYSTEM_TABLE;
typedef struct { uint32_t revision; EFI_HANDLE parent_handle; EFI_SYSTEM_TABLE *system_table; EFI_HANDLE device_handle; void *file_path,*reserved; uint32_t load_options_size; void *load_options; void *image_base; uint64_t image_size; uint32_t image_code_type,image_data_type; void *unload; } EFI_LOADED_IMAGE_PROTOCOL;
#define EFI_ERROR(s) (((s)>>63)!=0)
#define EFI_SUCCESS 0
#define EFI_BUFFER_TOO_SMALL 0x8000000000000005ULL
#define EFI_NOT_FOUND 0x800000000000000EULL
#define AllocateAnyPages 0
#define EfiLoaderData 2
#define EFI_FILE_MODE_READ 1ULL
