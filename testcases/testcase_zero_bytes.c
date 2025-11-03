#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test various zero-byte scenarios
    simplefs_create("test");
    int fd = simplefs_open("test");

    printf("Initial write 0 bytes at offset 0: %d\n", simplefs_write(fd, "", 0));
    simplefs_dump();

    printf("Seek to 10: %d\n", simplefs_seek(fd, 10));
    printf("Write 0 bytes at offset 10: %d\n", simplefs_write(fd, "", 0));
    simplefs_dump();

    printf("Write 1 byte: %d\n", simplefs_write(fd, "A", 1));
    simplefs_dump();

    simplefs_close(fd);
}
