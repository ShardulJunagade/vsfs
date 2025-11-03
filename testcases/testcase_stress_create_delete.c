#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();

    // Stress test: create and delete many times
    for (int cycle = 0; cycle < 3; cycle++)
    {
        printf("\n=== Cycle %d ===\n", cycle);
        
        // Create max files
        for (int i = 0; i < 8; i++)
        {
            char filename[10];
            sprintf(filename, "f%d", i);
            printf("Create %s: %d\n", filename, simplefs_create(filename));
        }
        
        // Delete all
        for (int i = 0; i < 8; i++)
        {
            char filename[10];
            sprintf(filename, "f%d", i);
            simplefs_delete(filename);
        }
        
        printf("After cycle %d:\n", cycle);
        simplefs_dump();
    }
}
