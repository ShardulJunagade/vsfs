#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test maximum inodes (8 files)
    for (int i = 0; i < 9; i++)
    {
        char filename[10];
        sprintf(filename, "f%d", i);
        int result = simplefs_create(filename);
        printf("Create %s: %d\n", filename, result);
    }
    
    printf("\nAfter creating 9 files (8th should fail):\n");
    simplefs_dump();
    
    // Delete one and try again
    simplefs_delete("f5");
    printf("\nAfter deleting f5:\n");
    simplefs_dump();
    
    printf("Create f9 after delete: %d\n", simplefs_create("f9"));
    printf("\nFinal state:\n");
    simplefs_dump();
}
