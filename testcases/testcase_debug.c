#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("debug");
    int fd = simplefs_open("debug");
    
    printf("File handle: %d\n", fd);

    // Try writing 1 byte
    char data1[] = "A";
    printf("Write 1 byte: %d\n", simplefs_write(fd, data1, 1));
    simplefs_dump();
    
    // Try writing 64 bytes
    char data64[64];
    memset(data64, 'B', 64);
    printf("Seek to 1: %d\n", simplefs_seek(fd, 1));
    printf("Write 64 bytes: %d\n", simplefs_write(fd, data64, 64));
    simplefs_dump();
    
    simplefs_close(fd);
}
