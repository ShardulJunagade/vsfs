#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test data integrity across operations
    simplefs_create("data");
    int fd = simplefs_open("data");

    // Write unique pattern
    char pattern[256];
    for (int i = 0; i < 256; i++)
    {
        pattern[i] = (char)(i % 128);
    }
    
    printf("Write 256 bytes of unique pattern: %d\n", simplefs_write(fd, pattern, 256));
    
    // Read back in different chunk sizes
    printf("Seek to 0: %d\n", simplefs_seek(fd, -256));
    
    char verify[256];
    
    // Read 17 bytes at a time (unaligned)
    int offset = 0;
    int errors = 0;
    while (offset < 256)
    {
        int to_read = (256 - offset) < 17 ? (256 - offset) : 17;
        printf("Read %d bytes at offset %d: %d\n", to_read, offset, simplefs_read(fd, verify + offset, to_read));
        printf("Seek by %d: %d\n", to_read, simplefs_seek(fd, to_read));
        offset += to_read;
    }
    
    // Verify data
    for (int i = 0; i < 256; i++)
    {
        if (verify[i] != pattern[i])
        {
            printf("ERROR at byte %d: expected %d, got %d\n", i, (unsigned char)pattern[i], (unsigned char)verify[i]);
            errors++;
            if (errors > 10) break; // Limit error output
        }
    }
    
    if (errors == 0)
    {
        printf("\n✓ Data integrity verified!\n");
    }
    else
    {
        printf("\n✗ Found %d data corruption errors\n", errors);
    }
    
    simplefs_close(fd);
}
