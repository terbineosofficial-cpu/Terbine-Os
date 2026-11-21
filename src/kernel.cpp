#include <stdint.h>
static inline void outb(uint16_t port,uint8_t value){asm volatile("outb %0,%1"::"a"(value),"Nd"(port));}
static inline uint8_t inb(uint16_t port){uint8_t v;asm volatile("inb %1,%0":"=a"(v):"Nd"(port));return v;}
static void serial_init(){outb(0x3F9,0);outb(0x3FB,0x80);outb(0x3F8,1);outb(0x3F9,0);outb(0x3FB,3);outb(0x3FA,0xC7);outb(0x3FC,0x0B);}
static void putc(char c){while(!(inb(0x3FD)&0x20)){}outb(0x3F8,(uint8_t)c);}
static void puts(const char *s){while(*s){if(*s=='\n')putc('\r');putc(*s++);}}
extern "C" __attribute__((section(".text.entry"),used)) void kmain(){
 serial_init();
 puts("\n[Terbine] nosktlnfc.exe started!\n");
 puts("[Terbine] Custom UEFI bootloader -> custom C++ kernel\n");
 puts("[Terbine] ExitBootServices succeeded.\n");
 puts("[Terbine] v0.0.1 ready (serial output only).\n");
 for(;;)asm volatile("hlt");
}
