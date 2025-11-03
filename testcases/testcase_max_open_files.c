#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Create 8 files
    for (int i = 0; i < 8; i++)
    {
        char filename[10];
        sprintf(filename, "file%d", i);
        simplefs_create(filename);
    }
    
    // Try to open more than MAX_OPEN_FILES
    int fds[25];
    int open_count = 0;
    
    for (int i = 0; i < 25; i++)
    {
        char filename[10];
        sprintf(filename, "file%d", i % 8);
        fds[i] = simplefs_open(filename);
        if (fds[i] != -1) open_count++;
        printf("Open %s (attempt %d): %d\n", filename, i+1, fds[i]);
    }
    
    printf("\nSuccessfully opened %d files\n", open_count);
    
    // Close all and try again
    for (int i = 0; i < 25; i++)
    {
        if (fds[i] != -1) simplefs_close(fds[i]);
    }
    
    printf("\nAfter closing all, open file0: %d\n", simplefs_open("file0"));
}
