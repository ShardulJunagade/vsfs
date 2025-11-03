#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("sparse");
    int fd = simplefs_open("sparse");

    // Write at offset 0
    char data1[] = "START";
    printf("Write 'START' at 0: %d\n", simplefs_write(fd, data1, 5));
    
    // Seek to middle of second block
    printf("Seek to 70: %d\n", simplefs_seek(fd, 70));
    
    // Try to write (should fail - can't create sparse files)
    char data2[] = "MIDDLE";
    printf("Write 'MIDDLE' at 70 (should fail): %d\n", simplefs_write(fd, data2, 6));
    
    // Seek to end of first write and continue
    printf("Seek to 5: %d\n", simplefs_seek(fd, -65));
    char data3[] = "_END";
    printf("Write '_END' at 5: %d\n", simplefs_write(fd, data3, 4));
    
    printf("\nFinal file:\n");
    simplefs_dump();
    
    simplefs_close(fd);
}
