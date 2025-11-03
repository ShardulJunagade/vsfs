#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test opening same file multiple times
    simplefs_create("shared");
    int fd1 = simplefs_open("shared");
    int fd2 = simplefs_open("shared");
    int fd3 = simplefs_open("shared");
    
    printf("First open: %d\n", fd1);
    printf("Second open: %d\n", fd2);
    printf("Third open: %d\n", fd3);
    
    // Write from first handle
    char data1[] = "FIRST";
    printf("Write 'FIRST' via fd1: %d\n", simplefs_write(fd1, data1, 5));
    
    // Seek on second handle
    printf("Seek fd2 to 5: %d\n", simplefs_seek(fd2, 5));
    
    // Write from second handle
    char data2[] = "_SECOND";
    printf("Write '_SECOND' via fd2: %d\n", simplefs_write(fd2, data2, 7));
    
    // Read from third handle (offset should be 0)
    char buf[13];
    buf[12] = '\0';
    printf("Read 12 bytes via fd3: %d\n", simplefs_read(fd3, buf, 12));
    printf("Data: '%s'\n", buf);
    
    simplefs_close(fd1);
    simplefs_close(fd2);
    simplefs_close(fd3);
    
    simplefs_dump();
}
