#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Create multiple files
    simplefs_create("file1");
    simplefs_create("file2");
    
    int fd1 = simplefs_open("file1");
    int fd2 = simplefs_open("file2");
    
    // Interleave writes
    printf("Write to file1: %d\n", simplefs_write(fd1, "AAA", 3));
    printf("Write to file2: %d\n", simplefs_write(fd2, "BBB", 3));
    printf("Write to file1: %d\n", simplefs_write(fd1, "AAA", 3));
    printf("Write to file2: %d\n", simplefs_write(fd2, "BBB", 3));
    
    // Interleave seeks
    printf("Seek file1 to 3: %d\n", simplefs_seek(fd1, 3));
    printf("Seek file2 to 0: %d\n", simplefs_seek(fd2, -3));
    
    // Interleave reads
    char buf1[4], buf2[4];
    buf1[3] = buf2[3] = '\0';
    printf("Read from file1: %d\n", simplefs_read(fd1, buf1, 3));
    printf("Read from file2: %d\n", simplefs_read(fd2, buf2, 3));
    printf("File1 data: '%s'\n", buf1);
    printf("File2 data: '%s'\n", buf2);
    
    simplefs_dump();
    
    simplefs_close(fd1);
    simplefs_close(fd2);
}
