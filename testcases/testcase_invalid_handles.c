#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test operations on invalid file handles
    char buf[64];
    
    printf("Read from invalid handle -1: %d\n", simplefs_read(-1, buf, 10));
    printf("Write to invalid handle -1: %d\n", simplefs_write(-1, buf, 10));
    printf("Seek invalid handle -1: %d\n", simplefs_seek(-1, 10));
    
    printf("Read from invalid handle 999: %d\n", simplefs_read(999, buf, 10));
    printf("Write to invalid handle 999: %d\n", simplefs_write(999, buf, 10));
    printf("Seek invalid handle 999: %d\n", simplefs_seek(999, 10));
    
    // Test operations on closed file handle
    simplefs_create("test");
    int fd = simplefs_open("test");
    printf("Valid fd: %d\n", fd);
    simplefs_close(fd);
    
    printf("Read after close: %d\n", simplefs_read(fd, buf, 10));
    printf("Write after close: %d\n", simplefs_write(fd, buf, 10));
    printf("Seek after close: %d\n", simplefs_seek(fd, 10));
}
