#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test edge case: try to read from empty file
    simplefs_create("empty");
    int fd = simplefs_open("empty");
    char buf[10];
    printf("Read from empty file: %d\n", simplefs_read(fd, buf, 5));

    // Test edge case: write 0 bytes
    printf("Write 0 bytes: %d\n", simplefs_write(fd, buf, 0));

    // Test edge case: seek beyond file size limit
    printf("Seek beyond limit: %d\n", simplefs_seek(fd, MAX_FILE_SIZE * BLOCKSIZE + 1));

    simplefs_close(fd);
    simplefs_dump();
}
