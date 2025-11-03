# Our Simple File System

**Due** Nov 3 by 11:59pm  
**Points** 15  
**Submitting** a file upload  
**File Types** c  
**Available** until Nov 3 at 11:59pm

## TL;DR

Download the starter code ↓, modify simplefs-ops.c and submit that file only as part of this assignment.

## Introduction

We will implement some basic file operations in a simple filesystem that runs over an emulated disk. You are provided a starter code ↓ with files simplefs-disk.h and simplefs-disk.c that emulate the disk, and provide functions to access the disk. Using these emulation functions, you will implement some basic filesystem operations like opening, reading, and writing files, by modifying the files simplefs-ops.c

The disk of the simple filesystem is emulated over a simple text file on disk, at a location specified in the code (say, the file "simplefs"). The emulated disk consists of 35 blocks of size 64 bytes each. The first block of the filesystem is the superblock, which contains a summary of the status of all other blocks in the system. The next 4 blocks of the disk store inodes, with two inodes per block. That is, the filesystem can store a total of 8 files on disk. The remaining 30 blocks are available to store file data. There is no directory hierarchy in the filesystem; all files are addressed simply by their filenames.

The inode structure stores direct pointers for up to 4 file data blocks. That is, the size of a file in our filesystem is restricted to 4 blocks or 256 bytes. In addition to the direct pointers to data blocks, the inode also stores the size of the file (in bytes) and the filename. The inode structure is 32 bytes in size, enabling the filesystem to pack two inodes into a disk block.

When you use the functions in simplefs-disk.h in your filesystem code, ensure that you take into account the return values from the functions to catch errors. All disk operations that do not return an error code should be assumed to have succeeded. For example, the operations of allocating a free data block may fail (if no blocks are free), but the operation of reading a valid data block is always expected to succeed. You are expected to catch and handle the errors for only those functions that return an error code.

Our simple filesystem has separate functions to create and open a file. Creating a file only creates it on disk, while opening a file opens an already created file for reading and writing. simplefs-disk.h defines an additional in-memory datastructure related to the filesystem, called the file handle array. The state of open files in the system (the inode number and the offset of reading/writing) is captured in a file handle of an open file. The filesystem maintains an array of all such file handles of open files, and the index of a file handle in this array is used to uniquely identify an open file in all subsequent read/write operations.

* `int simplefs_create(char *filename)` creates a file with a specified name in the filesystem. A file creation should succeed only if a file with the same name does not already exist in the filesystem, and if the system has space for an additional file to be created. During file creation, this function must allocate a free inode for the file and initialize it suitably on disk. This function returns the inode number of the newly created file on success, and -1 on a failure.

* `void simplefs_delete(char *filename)` deletes a file with the specified name from the filesystem (if it exists), and frees up the resources of the file such as its data blocks and its inode. You may assume that the file is closed before deleting it.

* `int simplefs_open(char *filename)` opens an existing file for reading and writing. This operation succeeds only if the file with the same name was previously created. This operation allocates an unused file handle from the global file handle array, initializes it suitably, and returns the index of the newly allocated file handle. This function returns -1 if the file open failed for any reason. Please note that an array of file handles has already been defined for you; you must populate an entry in this array when opening a file. All subsequent read and write operations on the file will have to obtain the inode number of the file using this returned file handle array index.

* `void simplefs_close(int file_handle)` closes an open file and frees up its file handle. This operation will not delete the file from disk.

* `int simplefs_read(int file_handle, char *buf, int nbytes)` reads the specified number of bytes from the current offset in an open file into the given buffer. The requested number of bytes can span multiple blocks on disk. This function returns 0 on success and -1 on failure. Note that we do not wish to support partial reads, i.e., your implementation must read all the requested number of bytes into the buffer, or none at all. If reading the requested number of bytes causes you to go beyond the end of the file, then this operation should simply not read anything and must return a failure. Note that you must handle the cases where reads are not aligned at the block boundary, and can start or end in the middle of a block. Note that reading a file will NOT result in updating the offset value in the file handle.


* `int simplefs_write(int file_handle, char *buf, int nbytes)` writes the specified number of bytes from the current offset in an open file to disk, from the given buffer. The requested number of bytes can span multiple blocks on disk. This function returns 0 on success and -1 on failure. Note that we do not wish to support partial writes, i.e., your implementation must write all the requested nbytes number of bytes into the file, or none at all. If the requested number of bytes cannot be written (e.g., writing the requested number of bytes causes you to go beyond the maximum file size limit, or there are no empty disk blocks available to complete the write) then this operation should simply not write anything and must return a failure. Further, on such failed writes, any data blocks that have been allocated before encountering the failure must

be freed up and returned back to the file system. That is, a failed write must leave the filesystem in the same state that it started with. Note that you must handle the cases where writes are not aligned at the block boundary, and can start or end in the middle of a block. Note that writing a file will NOT result in updating the offset value in the file handle.

* `int simplefs_seek(int file_handle, int nseek)` are used to implements the file offset in the file handle by nseek bytes. 0 with Linux filesystem, the offset indicates the next byte that can be read/written from the file. Note that the offset should decrease if nseek is negative. The seek operation should not move the offset to beyond the current file size boundaries. This function returns 0 on a success and -1 on a failure. Note that this is the only function that can change the current offset value from which the next read or write operation can happen.

All the above operations should implement changes to the on-disk datablocks of the emulated disk suitably, by only using the functions available to you in simplefs-disk.h.

## Testing

The folder testcases contains several sample test programs to test your filesystem implementation. Each test formats the file system, and performs several operations like creating, reading and writing files. Each program prints the success/failure status of each operation performed, and the final state of the disk after all operations have completed. You may compile and link any of these test programs with your simplefs C files to generate an executable to run. The folder expected output contains the output we expect your completed code will generate; this output was generated using our solution code.

After you complete your implementation, you should ensure that the output of your program matches the expected output exactly.

You can compile and run your code with a single testcase in the following manner (from the project root):

```
gcc -I . testcases/testcase0.c simplefs-ops.c simplefs-disk.c -o testcase0
./testcase0
```

Or compile and run any other testcase by replacing `testcase0.c` with the desired testcase file under the `testcases/` directory. The previous command failed for some setups because it assumed the testcase source was in the current directory; the commands above run from the project root and reference the `testcases/` folder explicitly.

To compile and run all provided testcases automatically, a helper script `run_all_testcases.sh` is included in the project root. It will compile each `testcases/*.c` with `simplefs-ops.c` and `simplefs-disk.c`, run the resulting binary, and print a short separator between test outputs.

Run the helper script like this:

```
./run_all_testcases.sh
```

If you need to inspect or re-run a single testcase, you can also compile it manually with the `gcc` command shown above.