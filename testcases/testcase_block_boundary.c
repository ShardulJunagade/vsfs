#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("boundary");
    int fd = simplefs_open("boundary");

    // Write exactly one block
    char block1[64];
    memset(block1, 'A', 64);
    printf("Write 64 bytes (1 block): %d\n", simplefs_write(fd, block1, 64));
    
    // Seek to start of second block
    printf("Seek to 64: %d\n", simplefs_seek(fd, 64));
    
    // Write second block
    char block2[64];
    memset(block2, 'B', 64);
    printf("Write 64 bytes at block boundary: %d\n", simplefs_write(fd, block2, 64));
    
    // Read across block boundary (offset is still at 64, seek back to 60)
    printf("Seek to 60: %d\n", simplefs_seek(fd, -4));
    char buf[10];
    printf("Read 10 bytes across boundary: %d\n", simplefs_read(fd, buf, 10));
    printf("Expected: AAAABBBBBB, Got: ");
    for(int i = 0; i < 10; i++) printf("%c", buf[i]);
    printf("\n");
    
    simplefs_dump();
    simplefs_close(fd);
}
