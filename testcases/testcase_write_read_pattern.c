#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("pattern");
    int fd = simplefs_open("pattern");

    // Write pattern: write, seek, write, seek...
    char data[] = "0123456789";
    
    for (int i = 0; i < 5; i++)
    {
        printf("Write 10 bytes at offset %d: %d\n", i*20, simplefs_write(fd, data, 10));
        printf("Seek by 20: %d\n", simplefs_seek(fd, 20));
    }
    
    // Now read back in different pattern
    printf("\nReading back:\n");
    printf("Seek to 0: %d\n", simplefs_seek(fd, -80));
    
    for (int i = 0; i < 5; i++)
    {
        char buf[11];
        buf[10] = '\0';
        printf("Read 10 bytes at offset %d: %d\n", i*20, simplefs_read(fd, buf, 10));
        printf("Data: '%s'\n", buf);
        printf("Seek by 20: %d\n", simplefs_seek(fd, 20));
    }
    
    simplefs_dump();
    simplefs_close(fd);
}
