#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test creating duplicate files
    printf("Create file1: %d\n", simplefs_create("file1"));
    printf("Create file1 again (should fail): %d\n", simplefs_create("file1"));
    
    // Test with exact 8-char filename
    printf("Create 12345678: %d\n", simplefs_create("12345678"));
    printf("Create 12345678 again (should fail): %d\n", simplefs_create("12345678"));
    
    // Test creating file with same name after deletion
    simplefs_delete("file1");
    printf("Create file1 after delete: %d\n", simplefs_create("file1"));
    
    simplefs_dump();
}
