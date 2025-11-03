#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    int result = simplefs_create("boundary");
    printf("Create 'boundary': %d\n", result);
    
    int fd = simplefs_open("boundary");
    printf("Open 'boundary': %d\n", fd);

    if (fd >= 0) {
        // Write exactly one block
        char block1[64];
        memset(block1, 'A', 64);
        printf("Write 64 bytes (1 block): %d\n", simplefs_write(fd, block1, 64));
        
        simplefs_dump();
        simplefs_close(fd);
    }
}
