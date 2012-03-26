// creates the file system
void mksfs(int fresh);
// lists files in the root directory
void sfs_ls();
// opens the given file
int sfs_fopen(char *name); 
// closes the given file
void sfs_fclose(int fileID);
// write buf characters into disk
void sfs_fwrite(int fileID, char *buf, int length);
// read characters from disk into buf
void sfs_fread(int fileID, char *buf, int length);
// seek to the location from beginning
void sfs_fseek(int fileID, int loc);
 // removes a file from the filesystem
int sfs_remove(char *file);

