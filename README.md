# famOS  

**famOS** is a multitasking operating system and kernel with an interactive shell (without GUI).  

![famOS](img/famOS_Logo.png)  

## Description  

This operating system consists of two parts, Real Mode and Protected Mode. 

### Real Mode  

Real mode is a legacy mode in all Intel processors that causes the processor to start in a legacy state, it performs like the old 8086 Intel processors did back in the way.
This part consists of boot process (boot loader), and memory logic.


### Protected Mode  

Protected Mode consists of 32 bit multi-tasking kernel that has the FAT16 filesystem. 
**famOS** will use Intel's built in memory protection and security mechanisms that allow us to instruct the processor to protect our kernel and prevent user programs from damaging it.  
This part consists of paging and virtual memory, memory virtualization, virtual filesystem (similar to the Linux kernel), FAT16 filesystem, keyboard drivers, memory management(design a customize heap) and ...   
