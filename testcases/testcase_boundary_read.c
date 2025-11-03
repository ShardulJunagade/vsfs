#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    simplefs_create("test");
    int fd = simplefs_open("test");

    // Write some data
    char data[] = "Hello World!";
    printf("Write 12 bytes: %d\n", simplefs_write(fd, data, 12));

    // Try to read exactly the file size (should succeed)
    char buf1[13];
    buf1[12] = '\0';
    printf("Read exactly file size (12 bytes): %d\n", simplefs_read(fd, buf1, 12));
    printf("Data: '%s'\n", buf1);

    // Try to read beyond file size (should fail)
    char buf2[14];
    printf("Read beyond file size (13 bytes): %d\n", simplefs_read(fd, buf2, 13));

    // Test reading 0 bytes (should succeed and do nothing)
    printf("Read 0 bytes: %d\n", simplefs_read(fd, buf2, 0));

    simplefs_close(fd);
}
