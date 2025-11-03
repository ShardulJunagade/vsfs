#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("maxfile");
    int fd = simplefs_open("maxfile");

    // Write exactly 256 bytes (max file size)
    char data[256];
    memset(data, 'X', 256);
    printf("Write 256 bytes (max): %d\n", simplefs_write(fd, data, 256));
    
    // Try to write one more byte (should fail)
    printf("Seek to 256: %d\n", simplefs_seek(fd, 256));
    char extra = 'Y';
    printf("Write 1 more byte (should fail): %d\n", simplefs_write(fd, &extra, 1));
    
    // Try to write at offset that would exceed max
    printf("Seek to 200: %d\n", simplefs_seek(fd, -56));
    char buf[100];
    memset(buf, 'Z', 100);
    printf("Write 100 bytes at offset 200 (should fail): %d\n", simplefs_write(fd, buf, 100));
    
    simplefs_dump();
    simplefs_close(fd);
}
