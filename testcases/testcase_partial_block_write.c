#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("partial");
    int fd = simplefs_open("partial");

    // Write 10 bytes in first block
    char data1[10];
    memset(data1, 'A', 10);
    printf("Write 10 bytes: %d\n", simplefs_write(fd, data1, 10));
    
    // Seek to byte 5 and write 20 bytes (crosses into second block)
    printf("Seek to 5: %d\n", simplefs_seek(fd, 5));
    char data2[20];
    memset(data2, 'B', 20);
    printf("Write 20 bytes at offset 5: %d\n", simplefs_write(fd, data2, 20));
    
    // Read everything
    printf("Seek to 0: %d\n", simplefs_seek(fd, -5));
    char buf[26];
    buf[25] = '\0';
    printf("Read 25 bytes: %d\n", simplefs_read(fd, buf, 25));
    
    printf("Expected pattern: AAAAABBBBBBBBBBBBBBBBBBBB\n");
    printf("Actual data:      %s\n", buf);
    
    simplefs_dump();
    simplefs_close(fd);
}
