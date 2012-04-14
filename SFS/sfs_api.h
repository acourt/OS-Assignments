void mksfs(int fresh);
void sfs_ls();
int sfs_fopen(char *name);
void sfs_fclose(int fileID);
void sfs_fwrite(int fileID, char *buf, int length);
void sfs_fread(int fileID, char *buf, int length);
void sfs_fseek(int fileID, int loc);
int sfs_remove(char *file);
