#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Delete non-existent file (should handle gracefully)
    printf("Deleting non-existent file 'ghost'\n");
    simplefs_delete("ghost");
    
    // Create, delete, then try to delete again
    simplefs_create("temp");
    printf("Created 'temp'\n");
    simplefs_dump();
    
    simplefs_delete("temp");
    printf("Deleted 'temp'\n");
    simplefs_dump();
    
    simplefs_delete("temp");
    printf("Deleted 'temp' again\n");
    simplefs_dump();
}
