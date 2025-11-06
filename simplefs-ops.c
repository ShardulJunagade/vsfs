#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
    // Check if file already exists
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    char target_name[MAX_NAME_STRLEN];
    memset(target_name, 0, sizeof(target_name));
    strncpy(target_name, filename, MAX_NAME_STRLEN);
    for(int i = 0; i < NUM_INODES; i++){
        simplefs_readInode(i, inode);
        if(inode->status == INODE_IN_USE && strncmp(inode->name, target_name, MAX_NAME_STRLEN) == 0){
            free(inode);
            return -1; // File already exists
        }
    }
    
    // Allocate a free inode
    int inode_num = simplefs_allocInode();
    if(inode_num == -1){
        free(inode);
        return -1; // No free inodes
    }
    
    // Initialize the inode
    inode->status = INODE_IN_USE;
    // Copy filename, ensuring it fits in MAX_NAME_STRLEN
    memset(inode->name, 0, MAX_NAME_STRLEN); // Clear the name field first
    strncpy(inode->name, filename, MAX_NAME_STRLEN);
    inode->file_size = 0;
    for(int i = 0; i < MAX_FILE_SIZE; i++){
        inode->direct_blocks[i] = -1;
    }
    
    // Write inode to disk
    simplefs_writeInode(inode_num, inode);
    free(inode);
    
    return inode_num;
}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    char target_name[MAX_NAME_STRLEN];
    memset(target_name, 0, sizeof(target_name));
    strncpy(target_name, filename, MAX_NAME_STRLEN);
    
    // Find the inode for the file
    for(int i = 0; i < NUM_INODES; i++){
        simplefs_readInode(i, inode);
        if(inode->status == INODE_IN_USE && strncmp(inode->name, target_name, MAX_NAME_STRLEN) == 0){
            // Free all data blocks
            for(int j = 0; j < MAX_FILE_SIZE; j++){
                if(inode->direct_blocks[j] != -1){
                    simplefs_freeDataBlock(inode->direct_blocks[j]);
                }
            }
            
            // Free the inode
            simplefs_freeInode(i);
            free(inode);
            return;
        }
    }
    
    free(inode);
}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    char target_name[MAX_NAME_STRLEN];
    memset(target_name, 0, sizeof(target_name));
    strncpy(target_name, filename, MAX_NAME_STRLEN);
    
    // Find the inode for the file
    int inode_num = -1;
    for(int i = 0; i < NUM_INODES; i++){
        simplefs_readInode(i, inode);
        if(inode->status == INODE_IN_USE && strncmp(inode->name, target_name, MAX_NAME_STRLEN) == 0){
            inode_num = i;
            break;
        }
    }
    
    free(inode);
    
    if(inode_num == -1){
        return -1; // File not found
    }
    
    // Allocate a file handle
    for(int i = 0; i < MAX_OPEN_FILES; i++){
        if(file_handle_array[i].inode_number == -1){
            file_handle_array[i].inode_number = inode_num;
            file_handle_array[i].offset = 0;
            return i;
        }
    }
    
    return -1; // No free file handles
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
    if(file_handle >= 0 && file_handle < MAX_OPEN_FILES){
        file_handle_array[file_handle].inode_number = -1;
        file_handle_array[file_handle].offset = 0;
    }
}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
    // Validate file handle
    if(file_handle < 0 || file_handle >= MAX_OPEN_FILES || 
       file_handle_array[file_handle].inode_number == -1){
        return -1;
    }
    
    // Handle zero-byte reads as success (no-op)
    if(nbytes == 0){
        return 0;
    }
    
    int inode_num = file_handle_array[file_handle].inode_number;
    int offset = file_handle_array[file_handle].offset;
    
    // Read inode
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    simplefs_readInode(inode_num, inode);
    
    // Check if read would go beyond file size
    if(offset + nbytes > inode->file_size){
        free(inode);
        return -1; // Cannot read beyond file size
    }
    
    // Read data block by block
    int bytes_read = 0;
    while(bytes_read < nbytes){
        int current_offset = offset + bytes_read;
        int block_index = current_offset / BLOCKSIZE;
        int block_offset = current_offset % BLOCKSIZE;
        int bytes_to_read = BLOCKSIZE - block_offset;
        if(bytes_to_read > nbytes - bytes_read){
            bytes_to_read = nbytes - bytes_read;
        }
        
        // Read the data block
        char temp_buf[BLOCKSIZE];
        simplefs_readDataBlock(inode->direct_blocks[block_index], temp_buf);
        memcpy(buf + bytes_read, temp_buf + block_offset, bytes_to_read);
        
        bytes_read += bytes_to_read;
    }
    
    free(inode);
    // Note: offset is NOT updated after read
    return 0;
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
    // Validate file handle
    if(file_handle < 0 || file_handle >= MAX_OPEN_FILES || 
       file_handle_array[file_handle].inode_number == -1){
        return -1;
    }
    
    // Handle zero-byte writes as success (no-op)
    if(nbytes == 0){
        return 0;
    }
    
    int inode_num = file_handle_array[file_handle].inode_number;
    int offset = file_handle_array[file_handle].offset;
    
    // Read inode
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    simplefs_readInode(inode_num, inode);
    
    // Check if write would exceed maximum file size
    if(offset + nbytes > MAX_FILE_SIZE * BLOCKSIZE){
        free(inode);
        return -1; // Would exceed max file size
    }
    
    // Track allocated blocks for rollback on failure
    int newly_allocated_blocks[MAX_FILE_SIZE];
    int newly_allocated_indices[MAX_FILE_SIZE];
    int num_newly_allocated = 0;
    for(int i = 0; i < MAX_FILE_SIZE; i++){
        newly_allocated_blocks[i] = -1;
        newly_allocated_indices[i] = -1;
    }
    
    // Determine which blocks are needed and allocate if necessary
    int start_block = offset / BLOCKSIZE;
    int end_block = (offset + nbytes - 1) / BLOCKSIZE;
    
    for(int i = start_block; i <= end_block; i++){
        if(inode->direct_blocks[i] == -1){
            int block_num = simplefs_allocDataBlock();
            if(block_num == -1){
                // Failed to allocate, rollback
                for(int j = 0; j < num_newly_allocated; j++){
                    simplefs_freeDataBlock(newly_allocated_blocks[j]);
                    inode->direct_blocks[newly_allocated_indices[j]] = -1;
                }
                free(inode);
                return -1;
            }
            inode->direct_blocks[i] = block_num;
            newly_allocated_blocks[num_newly_allocated] = block_num;
            newly_allocated_indices[num_newly_allocated] = i;
            num_newly_allocated++;
            
            // Initialize the new block with zeros
            char zero_buf[BLOCKSIZE];
            memset(zero_buf, 0, BLOCKSIZE);
            simplefs_writeDataBlock(block_num, zero_buf);
        }
    }
    
    // Write data block by block
    int bytes_written = 0;
    while(bytes_written < nbytes){
        int current_offset = offset + bytes_written;
        int block_index = current_offset / BLOCKSIZE;
        int block_offset = current_offset % BLOCKSIZE;
        int bytes_to_write = BLOCKSIZE - block_offset;
        if(bytes_to_write > nbytes - bytes_written){
            bytes_to_write = nbytes - bytes_written;
        }
        
        // Read existing data if we're writing partial block
        char temp_buf[BLOCKSIZE];
        if(block_offset != 0 || bytes_to_write != BLOCKSIZE){
            simplefs_readDataBlock(inode->direct_blocks[block_index], temp_buf);
        }
        
        // Copy new data
        memcpy(temp_buf + block_offset, buf + bytes_written, bytes_to_write);
        
        // Write back to disk
        simplefs_writeDataBlock(inode->direct_blocks[block_index], temp_buf);
        
        bytes_written += bytes_to_write;
    }
    
    // Update file size if necessary
    if(offset + nbytes > inode->file_size){
        inode->file_size = offset + nbytes;
    }
    
    // Write updated inode back to disk
    simplefs_writeInode(inode_num, inode);
    free(inode);
    
    // Note: offset is NOT updated after write
    return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
    // Validate file handle
    if(file_handle < 0 || file_handle >= MAX_OPEN_FILES || 
       file_handle_array[file_handle].inode_number == -1){
        return -1;
    }
    
    int inode_num = file_handle_array[file_handle].inode_number;
    int new_offset = file_handle_array[file_handle].offset + nseek;
    
    // Read inode to check file size
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    simplefs_readInode(inode_num, inode);
    
    // Check if new offset is within valid range
    if(new_offset < 0 || new_offset > inode->file_size){
        free(inode);
        return -1;
    }
    
    // Update offset
    file_handle_array[file_handle].offset = new_offset;
    free(inode);
    
    return 0;
}