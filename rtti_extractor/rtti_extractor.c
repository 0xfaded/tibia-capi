#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct ClassTypeInfo ClassTypeInfo;

struct ClassTypeInfo {
        int address;
        int parent;
        char *name;
};

int main(int argc, char **argv) {
   const uint32_t *rodata;
   int fd;
   struct stat sb;

   uint32_t base = (uint32_t)strtol(argv[2], NULL, 0);
   uint32_t class_type_info = (uint32_t)strtol(argv[3], NULL, 0);
   uint32_t si_class_type_info = (uint32_t)strtol(argv[4], NULL, 0);

   fd = open(argv[1], O_RDONLY);
   fstat(fd, &sb);

   rodata = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
   if (rodata == MAP_FAILED) {
           perror("mmap");
   }

   uint32_t i;
   for(i = 0; i < sb.st_size/4; i += 1) {
           if (rodata[i] == class_type_info) {
                   printf("%p\t%s\n", (void *)(base + 4*i), ((char *)rodata) + rodata[i+1] - base);
           } else if (rodata[i] == si_class_type_info) {
                   printf("%p\t%s\t%p\n", (void *)(base + 4*i), ((char *)rodata) + rodata[i+1] - base, (void *)rodata[i+2]);
           }
   }
   return 0;
}
