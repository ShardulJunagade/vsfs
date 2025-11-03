#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Fill up most data blocks to test rollback
    for (int i = 0; i < 7; i++)
    {
        char filename[10];
        sprintf(filename, "file%d", i);
        simplefs_create(filename);
        int fd = simplefs_open(filename);

        // Write 4 blocks per file (28 blocks total)
        char data[256];
        memset(data, 'A' + i, 256);
        simplefs_write(fd, data, 256);
        simplefs_close(fd);
    }

    // Now we have used 28 out of 30 data blocks
    printf("After creating 7 full files:\n");
    simplefs_dump();

    // Try to create a file and write 3 blocks (should fail after allocating 2 blocks)
    simplefs_create("testfail");
    int fd = simplefs_open("testfail");
    char bigdata[192]; // 3 blocks worth
    memset(bigdata, 'X', 192);
    printf("Attempt to write 3 blocks (should fail): %d\n", simplefs_write(fd, bigdata, 192));

    // Check that no blocks were allocated for the failed write
    printf("After failed write:\n");
    simplefs_dump();

    simplefs_close(fd);
}
