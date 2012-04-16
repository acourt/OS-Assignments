#define DISK_SIZE			256 // in blocks
#define BLOCK_SIZE			4096
#define MAX_FNAME_LENGTH	32
#define MAX_FILES			93

#define DIRECTORY_BLOCK_INDEX 0 
#define FAT_BLOCK_INDEX 1
#define FREE_BLOCK_LIST_INDEX 2 

#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sfs_api.h"
#include "disk_emu.h"

// On disk and memory
typedef struct  {
	char fname[MAX_FNAME_LENGTH];
	int size;
	int time;
	int FAT_index;
} dir_entry;

// On disk and memory
typedef struct {
	int start;
	int next;
} FAT_entry;

// Mem only
typedef struct  {
	int root;
	int read_block;		// FAT index for read
	int read_offset;
	int write_block;	// FAT index for write
	int write_offset;
}fd_entry;

fd_entry file_descriptors[MAX_FILES];
FAT_entry FAT[DISK_SIZE - 3]; // The size is the number of blocks minus the nes used for file system overhead
int free_list[DISK_SIZE - 3];

// Null entries are free
dir_entry directory[MAX_FILES];
int file_count = 0;



// Helper functions
void load_FAT_from_disk()
{
	// Construct the disk representation of the FAT
	char disk_FAT[BLOCK_SIZE];
	memset(disk_FAT, 0, BLOCK_SIZE);
	// Read it from disk
	read_blocks(FAT_BLOCK_INDEX, 1, disk_FAT);
	memcpy(FAT, disk_FAT, (DISK_SIZE - 3)*sizeof(FAT_entry));
}

void write_FAT_to_disk()
{
	// Construct the disk representation of the FAT
	char disk_FAT[BLOCK_SIZE];
	memset(disk_FAT, 0, BLOCK_SIZE);
	// Write it to disk
	memcpy(disk_FAT, FAT, (DISK_SIZE - 3)*sizeof(FAT_entry));
	write_blocks(FAT_BLOCK_INDEX, 1, disk_FAT);
}

void load_directory_from_disk()
{
	// Construct the disk representation of the directory
	char disk_directory[BLOCK_SIZE];
	memset(disk_directory,0, BLOCK_SIZE);
	// Read it from disk
	read_blocks(DIRECTORY_BLOCK_INDEX, 1, disk_directory);
	memcpy(&file_count, disk_directory, sizeof(int));
	memcpy(directory, disk_directory+4, MAX_FILES*sizeof(dir_entry));
}

// Write the entire contents of the directory
void write_directory_to_disk()
{
	// Construct the disk representation of the directory
	char disk_directory[BLOCK_SIZE];
	memset(disk_directory,0, BLOCK_SIZE);
	memcpy(disk_directory, &file_count, sizeof(int));
	memcpy(disk_directory+4, directory, MAX_FILES*sizeof(dir_entry));
	
	// Write it to disk
	write_blocks(DIRECTORY_BLOCK_INDEX, 1, directory);
}

void load_free_list_from_disk()
{
	// Construct the disk representation of the directory
	char disk_free_list[BLOCK_SIZE];
	memset(disk_free_list,0, BLOCK_SIZE);
	read_blocks(FREE_BLOCK_LIST_INDEX, 1, disk_free_list);
	memcpy(free_list, disk_free_list, DISK_SIZE - 3);

}

void write_free_list_to_disk()
{
	// Construct the disk representation of the directory
	char disk_free_list[BLOCK_SIZE];
	memset(disk_free_list,0, BLOCK_SIZE);
	memcpy(disk_free_list, free_list, DISK_SIZE - 3);
	write_blocks(FREE_BLOCK_LIST_INDEX, 1, disk_free_list);
}

int dir_entry_is_null(int entry)
{
	return directory[entry].fname[0] == '\0';
}

int get_current_time()
{
	time_t current_time;
	time(&current_time);
	return (int)current_time;
}

int get_free_block()
{
	int i=0, found_free_block = 0, free_block_index = -1;
	for (i=0; i < DISK_SIZE-3 && !found_free_block; i++) {
		// If there is a free spot
		if (free_list[i]) {
			free_list[i] = 0; // Mark it as occupied
			found_free_block = 1;
			free_block_index = i;
		}
	}
	return free_block_index;
}

int get_free_dir_index()
{
	int i, index = -1, found_free_index = 0;
	
	for ( i=0; i < MAX_FILES && !found_free_index; i++) {
		//printf("Directory name is: %s\n",directory[i].fname);
		if (dir_entry_is_null(i)) {
			//printf("Dir entry is null at %d\n",i);
			found_free_index = 1;
			index = i;
		}
	}
	
	return index;
}

// creates the file system
void mksfs(int fresh)
{
	if (fresh) {
		// Clear
		memset(FAT,0, sizeof(FAT_entry)*(DISK_SIZE-3));
		memset(free_list,1, sizeof(int)*(DISK_SIZE-3));
		memset(directory, 0, sizeof(dir_entry)*MAX_FILES);
		init_fresh_disk("DISK_FILE.txt", BLOCK_SIZE, DISK_SIZE);
	}
	else
	{
		init_disk("DISK_FILE.txt", BLOCK_SIZE, DISK_SIZE);
	}
	memset(file_descriptors, -1, sizeof(fd_entry)*MAX_FILES);
}

// lists files in the root directory
void sfs_ls()
{
	int i=0;
	// Linear print through the whole directory
	for (i=0; i < MAX_FILES; i++) {
		if(!dir_entry_is_null(i))
		{
			char* time = ctime((time_t*)&(directory[i].time));
			printf("Name: %s\tSize: %d\tLast Modified: %s\n", directory[i].fname, directory[i].size, time);
		}
	}
}

int get_block_count(int dir_index)
{
	// Find the block where the write pointer is
	int block_count=0;
	int next_FAT_index = file_descriptors[dir_index].root;
	while (FAT[next_FAT_index].next != -1) {
		//file_descriptors[dir_index].write_block = next_FAT_index;
		next_FAT_index = FAT[next_FAT_index].next;
		block_count++;
	}
	return block_count;
}

int get_fd(dir_index)
{
	// If the file descriptor exists
	if(is_file_open(dir_index))
	{
		file_descriptors[dir_index].root = directory[dir_index].FAT_index;
		// Append
		// Find the block where the write pointer is
		int next_FAT_index = file_descriptors[dir_index].root;
		while (FAT[next_FAT_index].next != -1) {
			file_descriptors[dir_index].write_block = next_FAT_index;
			next_FAT_index = FAT[next_FAT_index].next;
		}
		// Check the write offset
		file_descriptors[dir_index].write_offset = directory[dir_index].size % BLOCK_SIZE;
		
		// Read offsets
		file_descriptors[dir_index].read_block = file_descriptors[dir_index].root;
		file_descriptors[dir_index].read_offset = 0;
	}
	else
	{
		file_descriptors[dir_index].root = directory[dir_index].FAT_index;
		file_descriptors[dir_index].read_block = file_descriptors[dir_index].root;
		file_descriptors[dir_index].read_offset = 0;
		file_descriptors[dir_index].write_block = file_descriptors[dir_index].root;;
		file_descriptors[dir_index].write_offset = 0;
	}
	
	return dir_index;
}
int create_file_in_mem(char* name, int* dir_index );
int create_file_in_mem(char* name, int* dir_index )
{
	int block_index = get_free_block();
	int errorcode = 1;
	if (block_index == -1) {
		printf("Error: No more free blocks\n");
		errorcode =  -1;
	}
	else
	{
		//printf("Free block is %d\n", block_index);
		// Create the file
		FAT[block_index].start = block_index;
		FAT[block_index].next = -1;
		
		*dir_index = get_free_dir_index();
		if (*dir_index == -1) {
			printf("Error: Not enough space in the directory\n");
			return -1;
		}
		//printf("Free dir_index is %d\n", *dir_index);
		strncpy(directory[*dir_index].fname, name, MAX_FNAME_LENGTH);
		directory[*dir_index].FAT_index = block_index;
		directory[*dir_index].size = 0;
		directory[*dir_index].time = get_current_time();
		//printf("Printing file name: %s\n", directory[*dir_index].fname);

	}
	
	write_directory_to_disk();
	write_FAT_to_disk();
	write_free_list_to_disk();
	return errorcode;
}

// opens the given file
int sfs_fopen(char *name)
{
	int i=0;
	int dir_index = -1;
	int file_exists = 0;
	// Linear search through the directory
	for (i=0; i < MAX_FILES && !file_exists; i++) {
		if(!strcmp(directory[i].fname, name))
		{
			dir_index = i;
			file_exists = 1;
		}
	}
	if (!file_exists) {
		create_file_in_mem(name, &dir_index);
	}
	dir_index = get_fd(dir_index);	
	return dir_index;
}

// closes the given file
void sfs_fclose(int fileID)
{
	file_descriptors[fileID].root = -1;
}

int is_file_open(int dir_index)
{
	if (dir_index > MAX_FILES ||
		dir_index < 0) {
		return -1;
	}
	return (file_descriptors[dir_index].root != -1);	
}

// write buf characters into disk
void sfs_fwrite(int fileID, char *buf, int length)
{
	if (!is_file_open(fileID)) {
		printf("Error: Tried to write to a file without having it opened\n");
		return;
	}
	fd_entry* fd = &file_descriptors[fileID];
	int total_length = length;
	int file_has_new_blocks = 0;
	int old_write_offset = fd->write_offset;
	int old_block_offset = get_block_count(fileID);
	while (length > 0) 
	{
		char current_block[BLOCK_SIZE];
		read_blocks(FAT[fd->write_block].start + 3, 1, current_block);
		
		int available_space = BLOCK_SIZE - fd->write_offset;
		
		// Figure out if the available space is greater than the remaining bytes to write
		int min = available_space > length ? length : available_space;
		
		// Copy the information to the buffer
		memcpy(current_block+fd->write_offset , buf+(total_length-length) , min);
		//printf("%s\n", current_block+fd->write_offset);
		// Write the information contained in the buffer to the current block
		write_blocks(FAT[fd->write_block].start  + 3, 1,current_block);
		
		length -= min;
		
		/*printf("write_offset = %d\n", fd->write_offset);
		printf("min = %d\n", min);*/
		// Adjust the write offset
		fd->write_offset = (fd->write_offset +min ) % BLOCK_SIZE;
		
		if (fd->write_offset == 0) {
			int next_write_block = FAT[fd->write_block].next;
			if (next_write_block != -1) {
				// We are not at the end of the file
				fd->write_block = next_write_block;
			}
			else
			{
				int next_free_block = get_free_block();
				if (next_free_block ==-1) {
					printf("Error: No more free blocks!!\n");
					return ;
				}
				// Modify the FAT
				FAT[fd->write_block].next = next_free_block;
				FAT[next_free_block].start = next_free_block;
				FAT[next_free_block].next = -1;
				fd->write_block = next_free_block;
				file_has_new_blocks = 1;
			}
		}
	}
	
	int current_block_count = get_block_count(fileID);
	// If the size has changed
	/*printf("file has new blocks: %d\n", file_has_new_blocks);
	printf("old_block_offset ==  current_block_count  = %d\n",old_block_offset ==  current_block_count);
	printf("old_write_offset < fd->write_offset  = %d\n",old_write_offset < fd->write_offset);
	printf("Size is: %d + %d = %d\n", (current_block_count *  BLOCK_SIZE), fd->write_offset,
									(current_block_count *  BLOCK_SIZE)+ fd->write_offset);*/
	if (file_has_new_blocks
		|| (old_block_offset ==  current_block_count && old_write_offset < fd->write_offset))
	{
		directory[fileID].size = (current_block_count *  BLOCK_SIZE) + fd->write_offset;
	}
	
	write_directory_to_disk();
	write_FAT_to_disk();
	write_free_list_to_disk();
}

// read characters from disk into buf
void sfs_fread(int fileID, char *buf, int length)
{
	if (!is_file_open(fileID)) {
		printf("Error: Tried to read a file without having it opened\n");
		return;
	}
	fd_entry* fd = &file_descriptors[get_fd(fileID)];	
	int total_length = length;
	
	while (length > 0)
	{
		char current_block[BLOCK_SIZE];
		//printf("FAT[fd->read_block].start = %d\n", FAT[fd->read_block].start);
		read_blocks(FAT[fd->read_block].start + 3, 1, current_block);
		//printf("Got this from the block: %s\n", current_block);
		// Figure out if the available space is greater than the remaining bytes to write
		int min = BLOCK_SIZE - fd->read_offset > length ? length : BLOCK_SIZE - fd->read_offset;		
		
		memcpy(buf+(total_length-length), current_block+fd->read_offset, min);
		//printf("buffer: %s\n", buf+(total_length-length));
		length -= min;
		
		fd->read_offset = (fd->read_offset + min) % BLOCK_SIZE;
		
		if (fd->read_offset == 0) {
			int next_read_block = FAT[fd->read_block].next;
			if (next_read_block == -1) {
				printf("ERROR: No more blocks\n");
				return;
			}
			fd->read_block = next_read_block;
		}
	}
}

// seek to the location from beginning
void sfs_fseek(int fileID, int loc)
{
	if (file_descriptors[fileID].root == -1) {
		return;
	}
	file_descriptors[fileID].write_offset = loc%BLOCK_SIZE;
	file_descriptors[fileID].read_offset = loc%BLOCK_SIZE;
	printf("FileID is at %d\n", fileID);
	//printf("Seeking1, write_offset is now at: %d\n", file_descriptors[fileID].write_offset);
	int block_number = loc/BLOCK_SIZE;
	int current_block_index = file_descriptors[fileID].root;
	while (block_number != 0) {
		// Follow to the designated size node
		current_block_index = FAT[current_block_index].next;
		if (current_block_index == -1) {
			printf("Tried to seek past the end of file\n");
			return;
		}
		block_number--;
	}
	file_descriptors[fileID].read_block = current_block_index;
	file_descriptors[fileID].write_block = current_block_index;
	printf("write_offset at the end of seek: %d\n", file_descriptors[fileID].write_offset);
}
// removes a file from the filesystem
int sfs_remove(char *file)
{
	// Linear search through the directory
	int dir_index, file_exists = 0, i;
	// Linear search through the directory
	for (i=0; i < MAX_FILES && !file_exists; i++) {
		if(!strcmp(directory[i].fname, file))
		{
			dir_index = i;
			file_exists = 1;
		}
	}
	
	if (file_exists) {
		int block_index = directory[dir_index].FAT_index;
		while (block_index != -1) {
			free_list[block_index] = 0;
			block_index = FAT[block_index].next;
		}
		
		// Remove from directory
		memset(directory+dir_index, 0, sizeof(dir_entry));
		
		write_free_list_to_disk();
		write_directory_to_disk();
	}
	return 0;
}








