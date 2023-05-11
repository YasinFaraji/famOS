#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define FAMOS_TOTAL_INTERRUPTS 512

// 100 MB heap size
#define FAMOS_HEAP_SIZE_BYTES 104857600
#define FAMOS_HEAP_BLOCK_SIZE 4096
#define FAMOS_HEAP_ADDRESS 0x01000000 
#define FAMOS_HEAP_TABLE_ADDRESS 0x00007E00 

#define FAMOS_SECTOR_SIZE 512 

#define FAMOS_MAX_FILESYSTEMS 12 
#define FAMOS_MAX_FILE_DESCRIPTORS 512 

#define FAMOS_MAX_PATH 108 

#define FAMOS_TOTAL_GDT_SEGMENTS 3 

#endif