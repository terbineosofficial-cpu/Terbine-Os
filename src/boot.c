#include "efi.h"
static const EFI_GUID loaded_image_guid={0x5B1B31A1,0x9562,0x11d2,{0x8E,0x3F,0x00,0xA0,0xC9,0x69,0x72,0x3B}};
static const EFI_GUID fs_guid={0x964E5B22,0x6459,0x11d2,{0x8E,0x39,0x00,0xA0,0xC9,0x69,0x72,0x3B}};
static CHAR16 kernel_path[]={ '\\','n','o','s','k','t','l','n','f','c','.','e','x','e',0};
static void print(EFI_SYSTEM_TABLE *st,const CHAR16 *s){st->con_out->output_string(st->con_out,s);}
static int valid_header(const uint8_t *p){return p[0]=='T'&&p[1]=='B'&&p[2]=='K'&&p[3]=='1';}
static uint64_t u64(const uint8_t *p){uint64_t x=0;for(int i=7;i>=0;i--)x=(x<<8)|p[i];return x;}
typedef void (*kernel_entry_t)(void);
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image,EFI_SYSTEM_TABLE *st){
 EFI_BOOT_SERVICES *bs=st->boot_services;
 print(st,(const CHAR16*)u"Terbine Bootloader v0.0.1\r\n");
 EFI_LOADED_IMAGE_PROTOCOL *loaded=0;
 EFI_GUID li=loaded_image_guid,fs=fs_guid;
 EFI_STATUS s=bs->handle_protocol(image,&li,(void**)&loaded);
 if(EFI_ERROR(s)){print(st,(const CHAR16*)u"LoadedImage failed\r\n");return s;}
 EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *filesystem=0;
 s=bs->handle_protocol(loaded->device_handle,&fs,(void**)&filesystem);
 if(EFI_ERROR(s)){print(st,(const CHAR16*)u"Filesystem failed\r\n");return s;}
 EFI_FILE_PROTOCOL *root=0,*file=0;
 s=filesystem->open_volume(filesystem,&root);
 if(EFI_ERROR(s))return s;
 s=root->open(root,&file,kernel_path,EFI_FILE_MODE_READ,0);
 if(EFI_ERROR(s)){print(st,(const CHAR16*)u"Kernel not found\r\n");root->close(root);return s;}
 // Starter kernel is bounded to 1 MiB and read into pages that survive ExitBootServices.
 const size_t cap=1024*1024;
 uint64_t addr=0;
 s=bs->allocate_pages(AllocateAnyPages,EfiLoaderData,cap/4096,&addr);
 if(EFI_ERROR(s)){file->close(file);root->close(root);return s;}
 size_t read_size=cap;
 s=file->read(file,&read_size,(void*)(uintptr_t)addr);
 file->close(file);root->close(root);
 if(EFI_ERROR(s)||read_size<16||read_size==cap||!valid_header((uint8_t*)(uintptr_t)addr)){
  print(st,(const CHAR16*)u"Invalid or oversized kernel image\r\n");return EFI_NOT_FOUND;
 }
 uint8_t *bytes=(uint8_t*)(uintptr_t)addr;
 uint64_t offset=u64(bytes+8);
 if(offset>=read_size){print(st,(const CHAR16*)u"Invalid kernel entry\r\n");return EFI_NOT_FOUND;}
 print(st,(const CHAR16*)u"Kernel loaded. Exiting UEFI Boot Services...\r\n");
 size_t map_size=0,key=0,desc_size=0;uint32_t desc_version=0;
 s=bs->get_memory_map(&map_size,0,&key,&desc_size,&desc_version);
 if(s!=EFI_BUFFER_TOO_SMALL)return s;
 map_size+=desc_size*8;
 EFI_MEMORY_DESCRIPTOR *map=0;
 s=bs->allocate_pool(EfiLoaderData,map_size,(void**)&map);
 if(EFI_ERROR(s))return s;
 // No console/file/allocations between final GetMemoryMap and ExitBootServices.
 for(int tries=0;tries<5;tries++){
  size_t size=map_size;
  s=bs->get_memory_map(&size,map,&key,&desc_size,&desc_version);
  if(EFI_ERROR(s))return s;
  s=bs->exit_boot_services(image,key);
  if(!EFI_ERROR(s)){
   kernel_entry_t entry=(kernel_entry_t)(uintptr_t)(addr+offset);
   entry();
   for(;;)__asm__ volatile("cli; hlt");
  }
 }
 return s;
}
