#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("test");
    int fd = simplefs_open("test");

    // Write 100 bytes
    char data[100];
    memset(data, 'A', 100);
    printf("Write 100 bytes: %d\n", simplefs_write(fd, data, 100));

    // Test seek to exact file size (should succeed)
    printf("Seek to 100 (file size): %d\n", simplefs_seek(fd, 100));

    // Test seek beyond file size (should fail)
    printf("Seek to 101: %d\n", simplefs_seek(fd, 1));

    // Test seek back to 0
    printf("Seek back to start: %d\n", simplefs_seek(fd, -100));

    // Test seek to negative (should fail)
    printf("Seek to -1: %d\n", simplefs_seek(fd, -1));

    // Seek to middle
    printf("Seek to 50: %d\n", simplefs_seek(fd, 50));

    // Test large negative seek (should fail)
    printf("Seek by -1000: %d\n", simplefs_seek(fd, -1000));
    
    // Seek to 0 again and test reading
    printf("Seek to 0: %d\n", simplefs_seek(fd, -50));
    char buf[10];
    printf("Read 10 bytes: %d\n", simplefs_read(fd, buf, 10));

    simplefs_close(fd);
}
