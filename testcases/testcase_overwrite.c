#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("test");
    int fd = simplefs_open("test");

    // Write initial data
    char data1[] = "AAAAAAAAAAAAAAAA"; // 16 A's
    printf("Write 16 A's: %d\n", simplefs_write(fd, data1, 16));
    
    // Seek back and overwrite middle
    printf("Seek to 5: %d\n", simplefs_seek(fd, 5));
    char data2[] = "BBB";
    printf("Write 3 B's at offset 5: %d\n", simplefs_write(fd, data2, 3));
    
    // Read full file
    printf("Seek to 0: %d\n", simplefs_seek(fd, -5));
    char buf[17];
    buf[16] = '\0';
    printf("Read 16 bytes: %d\n", simplefs_read(fd, buf, 16));
    printf("Data: '%s'\n", buf);
    
    simplefs_dump();
    simplefs_close(fd);
}
