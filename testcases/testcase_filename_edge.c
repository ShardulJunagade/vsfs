#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Test empty filename
    printf("Create empty name: %d\n", simplefs_create(""));
    
    // Test single char
    printf("Create 'a': %d\n", simplefs_create("a"));
    
    // Test exactly 8 chars
    printf("Create '12345678': %d\n", simplefs_create("12345678"));
    
    // Test 9 chars (should be truncated)
    printf("Create '123456789': %d\n", simplefs_create("123456789"));
    
    // Test if truncated name collides
    printf("Create '12345678' again: %d\n", simplefs_create("12345678"));
    printf("Create '12345670': %d\n", simplefs_create("12345670"));
    
    simplefs_dump();
    
    // Test opening with different lengths
    printf("\nOpen 'a': %d\n", simplefs_open("a"));
    printf("Open '12345678': %d\n", simplefs_open("12345678"));
    printf("Open '123456789' (should find '12345678'): %d\n", simplefs_open("123456789"));
}
