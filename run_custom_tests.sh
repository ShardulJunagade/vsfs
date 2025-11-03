#!/bin/bash
# Script to compile and run all custom test cases

echo "======================================================"
echo "Simple File System - Custom Test Suite Runner"
echo "======================================================"
echo

cd "$(dirname "$0")"

# Array of test cases
tests=(
    "testcase_empty_read"
    "testcase_rollback"
    "testcase_zero_bytes"
    "testcase_boundary_read"
    "testcase_duplicate_create"
    "testcase_max_inodes"
    "testcase_invalid_handles"
    "testcase_seek_boundaries"
    "testcase_overwrite"
    "testcase_sparse_write"
    "testcase_max_file_size"
    "testcase_multiple_opens"
    "testcase_delete_nonexistent"
    "testcase_open_nonexistent"
    "testcase_block_boundary"
    "testcase_max_open_files"
    "testcase_filename_edge"
    "testcase_interleaved_ops"
    "testcase_partial_block_write"
    "testcase_write_read_pattern"
    "testcase_stress_create_delete"
    "testcase_data_integrity"
)

pass_count=0
fail_count=0
total=${#tests[@]}

for test in "${tests[@]}"; do
    echo "Testing: $test"
    echo "----------------------------"
    
    # Compile
    if gcc -I . testcases/${test}.c simplefs-ops.c simplefs-disk.c -o bin/${test} 2>&1 | grep -i error; then
        echo "❌ Compilation failed for $test"
        ((fail_count++))
    else
        # Run
        if timeout 5s ./bin/${test} > /dev/null 2>&1; then
            echo "✓ $test passed"
            ((pass_count++))
        else
            echo "❌ $test failed or timed out"
            ((fail_count++))
        fi
    fi
    echo
done

echo "======================================================"
echo "Test Summary"
echo "======================================================"
echo "Total tests: $total"
echo "Passed: $pass_count"
echo "Failed: $fail_count"
echo "Success rate: $((pass_count * 100 / total))%"
echo "======================================================"
