#ifndef MMAP_H
#define MMAP_H

#include <stddef.h>
#include <sys/types.h> 

#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define MAP_PRIVATE 0x2
#define MAP_ANONYMOUS 0x20

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);

#endif
