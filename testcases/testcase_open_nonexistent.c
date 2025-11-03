#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Try to open non-existent file
    int fd1 = simplefs_open("nonexistent");
    printf("Open non-existent file: %d\n", fd1);
    
    // Try to open after creating
    simplefs_create("exists");
    int fd2 = simplefs_open("exists");
    printf("Open existing file: %d\n", fd2);
    
    // Try to open after deleting
    simplefs_delete("exists");
    int fd3 = simplefs_open("exists");
    printf("Open deleted file: %d\n", fd3);
    
    if (fd2 >= 0) simplefs_close(fd2);
}
