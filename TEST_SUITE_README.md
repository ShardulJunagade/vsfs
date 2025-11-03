# Comprehensive Test Suite for Simple File System

## Test Files Created

### 1. **testcase_empty_read.c** - Edge Case: Empty File Operations
- Tests reading from an empty file (should fail)
- Tests writing 0 bytes
- Tests seeking beyond file size limit

### 2. **testcase_rollback.c** - Critical: Write Rollback on Failure
- Fills 28 of 30 data blocks with 7 full files
- Attempts to write 3 blocks when only 2 are available
- Verifies that partial allocation is rolled back on failure
- **Tests the all-or-nothing write guarantee**

### 3. **testcase_zero_bytes.c** - Edge Case: Zero-Byte Operations
- Tests writing 0 bytes at various offsets
- Tests seeking and writing 0 bytes
- Verifies no spurious block allocation

### 4. **testcase_boundary_read.c** - Boundary Testing: Read Limits
- Tests reading exactly file size (should succeed)
- Tests reading beyond file size (should fail)
- Tests reading 0 bytes (should succeed)

### 5. **testcase_duplicate_create.c** - Error Handling: Duplicate Files
- Tests creating a file with an existing name (should fail)
- Tests with exact 8-character filenames
- Tests creating after deletion

### 6. **testcase_max_inodes.c** - Resource Limit: Maximum Files
- Attempts to create 9 files (max is 8)
- Tests deletion and recreation
- Verifies inode allocation/deallocation

### 7. **testcase_invalid_handles.c** - Error Handling: Invalid Operations
- Tests operations with invalid handles (-1, 999)
- Tests operations after file close
- Verifies proper validation

### 8. **testcase_seek_boundaries.c** - Seek Edge Cases
- Tests seeking to exact file size
- Tests seeking beyond file size (should fail)
- Tests seeking to negative offsets (should fail)
- Tests seeking back to 0

### 9. **testcase_overwrite.c** - Data Integrity: Overwriting
- Writes initial data
- Seeks back and overwrites middle portion
- Verifies data integrity after partial overwrite

### 10. **testcase_sparse_write.c** - Error Handling: Sparse Files
- Tests writing at non-contiguous offsets
- Verifies that sparse files are not supported
- Tests proper sequent ial writes

### 11. **testcase_max_file_size.c** - Boundary: Maximum File Size
- Writes exactly 256 bytes (4 blocks)
- Attempts to write beyond limit (should fail)
- Tests write that would span beyond max size

### 12. **testcase_multiple_opens.c** - Concurrency: Multiple Handles
- Opens same file multiple times
- Writes/reads from different handles
- Verifies independent offset management

### 13. **testcase_delete_nonexistent.c** - Error Handling: Delete
- Deletes non-existent file (should handle gracefully)
- Double deletion test
- Verifies robust error handling

### 14. **testcase_open_nonexistent.c** - Error Handling: Open
- Opens non-existent file (should fail)
- Opens deleted file (should fail)
- Verifies proper file existence checks

### 15. **testcase_block_boundary.c** - Block Alignment Testing
- Writes aligned blocks (64 bytes)
- Reads across block boundaries
- Verifies proper block boundary handling

### 16. **testcase_max_open_files.c** - Resource Limit: File Handles
- Attempts to open more than MAX_OPEN_FILES (20)
- Tests handle reuse after close
- Verifies file handle management

### 17. **testcase_filename_edge.c** - Filename Edge Cases
- Tests empty filename
- Tests single character filename
- Tests exactly 8 character filenames
- Tests truncation of longer names
- Tests collision after truncation

### 18. **testcase_interleaved_ops.c** - Concurrency: Interleaved Operations
- Creates multiple files
- Interleaves writes, seeks, and reads
- Verifies proper state isolation

### 19. **testcase_partial_block_write.c** - Unaligned Writes
- Writes 10 bytes
- Writes 20 bytes starting at offset 5 (spans 2 blocks)
- Verifies proper handling of partial block writes

### 20. **testcase_write_read_pattern.c** - Pattern Testing
- Writes pattern: 10 bytes, seek 20, repeat
- Reads back in different pattern
- Verifies data integrity with complex access patterns

### 21. **testcase_stress_create_delete.c** - Stress Testing
- Creates maximum files, deletes all
- Repeats 3 cycles
- Verifies resource cleanup and reuse

### 22. **testcase_data_integrity.c** - Critical: Data Integrity
- Writes 256 bytes of unique pattern
- Reads back in 17-byte chunks (unaligned)
- Verifies byte-by-byte correctness
- **Most comprehensive data integrity test**

## Test Categories

### Critical Tests (Must Pass)
- testcase_rollback.c
- testcase_data_integrity.c
- testcase_overwrite.c

### Resource Management
- testcase_max_inodes.c
- testcase_max_open_files.c
- testcase_max_file_size.c
- testcase_stress_create_delete.c

### Error Handling
- testcase_invalid_handles.c
- testcase_duplicate_create.c
- testcase_delete_nonexistent.c
- testcase_open_nonexistent.c
- testcase_sparse_write.c

### Boundary/Edge Cases
- testcase_empty_read.c
- testcase_zero_bytes.c
- testcase_boundary_read.c
- testcase_seek_boundaries.c
- testcase_block_boundary.c
- testcase_filename_edge.c

### Data Correctness
- testcase_partial_block_write.c
- testcase_write_read_pattern.c
- testcase_interleaved_ops.c
- testcase_multiple_opens.c

## Key Bugs These Tests May Catch

1. **Incomplete Rollback**: testcase_rollback.c catches if allocated blocks aren't freed on write failure
2. **Off-by-One Errors**: Various boundary tests catch offset calculation errors
3. **Data Corruption**: testcase_data_integrity.c catches any data misalignment or corruption
4. **Resource Leaks**: Stress tests catch inode/block/handle leaks
5. **Improper Validation**: Invalid handle tests catch missing error checks
6. **Block Boundary Issues**: testcase_block_boundary.c catches read/write errors at block edges
7. **Filename Handling**: testcase_filename_edge.c catches truncation and comparison bugs
8. **Offset Management**: Multiple tests verify that only seek() updates offsets

## Running the Tests

Individual test:
```bash
gcc -I . testcases/testcase_name.c simplefs-ops.c simplefs-disk.c -o bin/testcase_name
./bin/testcase_name
```

All tests:
```bash
./run_all_testcases.sh
```

## Expected Behavior Summary

- **Create**: Returns inode number on success, -1 if file exists or no inodes available
- **Open**: Returns file handle index, -1 if file doesn't exist or no handles available
- **Close**: Always succeeds for valid handles
- **Delete**: Frees all resources, handles non-existent files gracefully
- **Read**: Returns 0 on success, -1 if would exceed file size or invalid handle
- **Write**: Returns 0 on success, -1 if would exceed max size or insufficient blocks
- **Seek**: Returns 0 on success, -1 if new offset would be out of bounds [0, file_size]

## Important Implementation Notes

1. Only `seek()` modifies file offset
2. `read()` and `write()` do NOT update offset
3. Write failures must rollback ALL allocated blocks
4. No partial reads/writes - all or nothing
5. Maximum file size: 256 bytes (4 blocks × 64 bytes)
6. Maximum files: 8 (NUM_INODES)
7. Maximum open files: 20 (MAX_OPEN_FILES)
8. Filename length: 8 characters (MAX_NAME_STRLEN)
